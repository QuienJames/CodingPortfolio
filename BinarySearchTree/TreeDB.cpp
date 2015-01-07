#include "TreeDB.h"

TreeDB::TreeDB(){
	//	cout<<"TreeDB constructed"<<endl;
	root = NULL;
	probesCount = 0;
}

TreeDB::~TreeDB(){
	//	cout<<"TreeDB destructed"<<endl;
	clear();
	delete root;
	//Is to empty the whole node to essentially nothing	
}

bool TreeDB::insert(DBentry* newEntry){
	if(find(newEntry->getName())!=NULL){
		delete newEntry;
		return false;
	}
	else{
		TreeNode* inserter = root;
		if (root == NULL)
			root = new TreeNode(newEntry);
		else
			insertHelper(inserter, newEntry, newEntry->getName());
		return true;
	}
}

void TreeDB::insertHelper(TreeNode* inserter_, DBentry* newEntry_, string name_){
	if(inserter_ == NULL)
		inserter_ = new TreeNode(newEntry_);	
	else if(name_ < inserter_->getEntry()->getName()){
		if (inserter_->getLeft()==NULL)
			inserter_->setLeft(new TreeNode(newEntry_));
		else
			insertHelper(inserter_->getLeft(), newEntry_, name_);
	}
	else{
		if (inserter_->getRight()==NULL)
			inserter_->setRight(new TreeNode(newEntry_));
		else
			insertHelper(inserter_->getRight(), newEntry_, name_);
	}
}
			
DBentry* TreeDB::find(string name){
	TreeNode* finder = root;
	probesCount = 0;
	if (root == NULL)
		return NULL;
	else
		return finderHelper(name, finder);
}
		
DBentry* TreeDB::finderHelper(string name_, TreeNode* finder_){
	if (finder_ == NULL)
		return NULL;
	else if (finder_->getEntry()->getName()==name_){
		probesCount++;
		return finder_->getEntry();
	}		
	else if (noProbesFinderHelper(name_, finder_->getLeft())!=NULL){
		probesCount++;
		return finderHelper(name_, finder_->getLeft());
	}	
	else if (noProbesFinderHelper(name_, finder_->getRight())!=NULL){
		probesCount++;
		return finderHelper(name_, finder_->getRight());
	}
	else
		return NULL;
}
	
DBentry* TreeDB::noProbesFinderHelper(string name_, TreeNode* finder_){
	if (finder_ == NULL)
		return NULL;
	else if (finder_->getEntry()->getName()==name_){
		return finder_->getEntry();
	}		
	else if (noProbesFinderHelper(name_, finder_->getLeft())!=NULL){
		return noProbesFinderHelper(name_, finder_->getLeft());
	}	
	else if (noProbesFinderHelper(name_, finder_->getRight())!=NULL){
		return noProbesFinderHelper(name_, finder_->getRight());
	}
	else
		return NULL;
}

void TreeDB::countActive() const{
	TreeNode* finder = root;
	cout<<countHelper(finder)<<endl;
}

int TreeDB::countHelper(TreeNode* current) const{
	if (current == NULL)
		return 0;
	else if (current->getEntry()->getActive())
		return countHelper(current->getLeft()) + countHelper(current->getRight()) + 1;
	else 
		return countHelper(current->getLeft()) + countHelper(current->getRight());
}
	
void TreeDB::updateStatus (string name_, bool status_){
	TreeNode* finder = root;

	if(find(name_)==NULL)
		cout<<"Error: entry does not exist"<<endl;
	else{
		find(name_)->setActive(status_);
		cout<<"Success"<<endl;
	}
}
	
void TreeDB::printAll(){
	TreeNode* printer = root;
	if(root == NULL)
		return;
	else
		printHelper(printer);
}

void TreeDB::printHelper(TreeNode* current_){
	if (current_ == NULL)
		return;
	else{//print to the left
		printHelper(current_->getLeft());
		//print to the current
		cout<<current_->getEntry()->getName()<<" : "<<current_->getEntry()->getIPaddress()<<" : ";
		if(current_->getEntry()->getActive())
			cout<<"active"<<endl;
		else
			cout<<"inactive"<<endl;
		//print to the right
		printHelper(current_->getRight());
	}
}

void TreeDB::clear(){
	if (root == NULL) 
		return;
	else{
		root->deleteAll();
		root = NULL;
	}
}

bool TreeDB::remove(string name){
	
	if (find(name)==NULL) 
		return false;
	else{
		root->deleteNode(name, root);
		return true;
	}	
}
/*
TreeNode* TreeDB::returnNode(string name_, TreeNode* finder_){
	if (finder_->getEntry()->getName()==name_)
		return finder_;	
	else if (returnNode(name_, finder_->getLeft())!=NULL)
		return returnNode(name_, finder_->getLeft());
	else if (returnNode(name_, finder_->getRight())!=NULL)
		return returnNode(name_, finder_->getRight());
	else
		return NULL;
}
//A function that can return a pointer to a node based on the name 
//Useful, but not particularly necessary for this assignment
*/	

void TreeDB::printProbes() const{
	cout<<probesCount<<endl;
}


	
	
	
