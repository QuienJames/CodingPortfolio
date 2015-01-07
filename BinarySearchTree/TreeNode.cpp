#include "TreeNode.h"

TreeNode::TreeNode(DBentry* _entryPtr){
	entryPtr = _entryPtr;
	left = NULL;
	right = NULL;
		//cout<<"TreeNode constructed"<<endl;
}

TreeNode::~TreeNode(){
		//cout<<"TreeNode destructed"<<endl;
	delete entryPtr;
}

void TreeNode::deleteNode(string name_, TreeNode* &ptr){	
	if(name_<getEntry()->getName()){
		if (left!=NULL){
			left->deleteNode(name_, left);
			return;
		}
	}
	 else if (name_>getEntry()->getName()){
		if(right!=NULL){
			right->deleteNode(name_, right);
			return;
		}
	}
	else if (name_==getEntry()->getName()){
		if((left==NULL)&&(right==NULL)){
			ptr = NULL;
			delete this;
			return;
		}   
		else if ((left==NULL)&&(right!=NULL)){
			ptr = right;
			right = NULL;
			delete this;
			return;
		}
		else if((left!=NULL)&&(right==NULL)){
			ptr = left;
			left = NULL;
			delete this;
			return;
		}
		else{
			entryPtr = left->findMax();
			left->deleteNode(entryPtr->getName(), left);
			return;
		}
	}
}

DBentry* TreeNode::findMax(){
	if (right==NULL)
		return entryPtr;
	else
		return right->findMax();
}
		
void TreeNode::deleteAll(){
	if(left!=NULL)
		left->deleteAll();
	if(right!=NULL)
		right->deleteAll();
	delete this;
}

void TreeNode::setLeft(TreeNode* newLeft){
	left = newLeft;
}

void TreeNode::setRight(TreeNode* newRight){
	right = newRight;
}

TreeNode* TreeNode::getLeft() const{
	return left;
}

TreeNode* TreeNode::getRight() const{
	return right;
}

DBentry* TreeNode::getEntry() const{
	return entryPtr;
}

