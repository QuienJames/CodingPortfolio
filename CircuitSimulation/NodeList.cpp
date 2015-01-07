#include "NodeList.h"
#include "Node.h" 
#include "ResistorList.h"

#define MIN_ITERATION_CHANGE 0.0001

NodeList::NodeList(){
	head = NULL;
}	
		
NodeList::~NodeList(){	
	while (head!=NULL){
		Node* temp = head;
		head = head->getNext();
		delete temp;
	}	
	delete head;
}
	
bool NodeList::printNode(int nodeID_){
	if (findNode(nodeID_)){
		Node* finder = head;
	
		while (finder!=NULL&&finder->getNodeIndex()!=nodeID_)
			finder = finder->getNext();
		
		if(finder->getResList()){
			cout<<"Print:"<<endl;
			finder->print();
			return true;
		}
		else
			return false;
	}
	//find node and print it then return true
	else
	return false;
}

bool NodeList::findNode (int nodeID_){
	Node* finder = head;
	
	while (finder!=NULL&&finder->getNodeIndex()!=nodeID_)
		finder = finder->getNext();
	
	if (finder== NULL)
		return false;
	else if (finder->getNodeIndex()==nodeID_)
		return true;
	else
	return false;
}
	
void NodeList::insertNode(Node* newPtr){
	int newNodeIndex = newPtr -> getNodeIndex();
	
	Node* current = head;
	Node* previous = NULL;

	while (current!=NULL&&(current->getNodeIndex()<newNodeIndex)){
		previous = current;
		current = current -> getNext();
	}
	newPtr -> setNext(current);
	
	if (previous == NULL)
		head = newPtr;
	else
		previous -> setNext(newPtr);
}

void NodeList::printAllNodes(){
	Node* temp = head;
	
	cout<<"Print:"<<endl;
	
	while (temp!=NULL){
		temp->print();
		temp = temp->getNext();
	}
}

void NodeList::printResistor (string resistorName_){
	Node* finder = head;
   	bool isResistorThere = false;
	
	while (finder!=NULL&&!isResistorThere){
		if (finder->findResistor(resistorName_))
			isResistorThere = true;
		else
		finder = finder->getNext();
	}
	
	if (isResistorThere)
		finder->printRes(resistorName_);
	else
		;
}
	
void NodeList::addResistor(int nodeID_,Resistor* newRes){
	if (findNode(nodeID_)){
		Node* finder = head;
		
		while (finder!=NULL&&finder->getNodeIndex()!=nodeID_)
			finder = finder->getNext();
			
		finder->addResistor(newRes);
		return;
	}
	else
	return;
}

bool NodeList::findAndSetResistorList (string resistorName_, double newResistance_){
   	Node* finder = head;
   	bool isResistorThere = false;
	
	while (finder!=NULL){
		if (finder->findAndSetResistor(resistorName_, newResistance_))
			isResistorThere = true;
			
		finder = finder->getNext();
	}
	if (isResistorThere==false)
		return false;
	else
		return true;
}

bool NodeList::findResistor (string resistorName_){
	Node* finder = head;
   	bool isResistorThere = false;
	
	while (finder!=NULL){
		if (finder->findResistor(resistorName_))
			isResistorThere = true;
			
		finder = finder->getNext();
	}
	if (isResistorThere==false)
		return false;
	else
		return true;
}

void NodeList::deleteResistor (string resistorName_){
	Node* finder = head;
	
	while (finder!=NULL){
		if (finder->findResistor(resistorName_))
			finder->deleteResistor(resistorName_);
			
		finder = finder->getNext();
	}
	return;
}


double NodeList::returnOldResistance (string resistorName_){
	Node* finder = head;
	
	while (finder!=NULL&&!(finder->findResistor(resistorName_)))
		finder = finder->getNext();
	
		return finder->returnResistance (resistorName_);
}


void NodeList::deleteAllResistors(){
	Node* deleterRes = head;
	
	while (deleterRes!=NULL){
		deleterRes -> deleteAllResistors();
		deleterRes = deleterRes -> getNext();
	}
}

void NodeList::setV(double voltage_, int nodeID_){
	Node* finder = head;
		
	while (finder!=NULL&&finder->getNodeIndex()!=nodeID_)
		finder = finder->getNext();
	if(finder==NULL)
		return;
	else
		finder->setV(voltage_);
}

void NodeList::unset(int nodeID_){
	Node* finder = head;
		
	while (finder!=NULL&&finder->getNodeIndex()!=nodeID_)
		finder = finder->getNext();
	if(finder==NULL)
		return;
	else
	finder->unset();
}
	
bool NodeList::checkAnyVoltageSet(){
	Node* finder = head;
		
	while (finder!=NULL&&!finder->isVoltageSetNode())
		finder = finder->getNext();
		
	if(finder==NULL)
		return false;
	else
		return true;
}

bool NodeList::solve(){
	Node* finder = head;
	
	if(!checkAnyVoltageSet()){
		cout<<"Error: no nodes have their voltage set"<<endl;
		return false;
	}
	else{
		while(finder!=NULL){
			if(!finder->isVoltageSetNode())
				finder->changeVoltage(0);
			finder = finder->getNext();
		}
		
			newVoltageValue();
								
		while(someGreaterThanMinItChange()){
			newVoltageValue();		
		}
		
	//Solve function follows through Gauss-Seidel method of solving for node voltages and all functions 
	//used enable this method
		return true;
	}
}
	
bool NodeList::someGreaterThanMinItChange(){
	Node* finder = head;
	bool checkIterationChange = false;
	
	while (finder!=NULL&&!checkIterationChange){
		if((finder->getDeltaV()>MIN_ITERATION_CHANGE)&&!(finder->isVoltageSetNode())){
			checkIterationChange = true;
			}
		else
			finder = finder->getNext();
	}
	return checkIterationChange;
}


string NodeList::findCommonRes(Node* finder_,Node* current_){
	string none = "NoResistorFoundAtAllMoveOnToTheNextOneAndContinueExecution";
	int numResCurrent = current_->returnNumResNode();
	int numResFinder = finder_->returnNumResNode();
	
	for (int i = 1; i<=numResCurrent;i++){
		for(int j = 1; j<=numResFinder;j++){
			if(current_->returnNthResistor(i)==finder_->returnNthResistor(j))
				return current_->returnNthResistor(i);
		}
	}
	
	return none;
}

double NodeList::secondTerm(const int currentNodeID){
	double value=0; 
	double holder, numerator, denominator;
	string resName;
	Node* finder = head;
	
	Node* current = head;
	while ((current!=NULL)&&(currentNodeID!=current->getNodeIndex())){
		current = current->getNext();
	}
	
	while(finder!=NULL){
		if(finder->getNodeIndex()==currentNodeID)
			finder = finder->getNext();
		else{	
			resName = findCommonRes(finder,current);
			
			if (resName!="NoResistorFoundAtAllMoveOnToTheNextOneAndContinueExecution"){
				numerator = finder->getVoltage();
				denominator = finder->returnResistance(resName);
				holder = numerator/denominator; 
				value = value + holder;
				finder = finder->getNext();
			}
			else
				finder = finder->getNext();
		}
	}
	return value;
}

void NodeList::newVoltageValue(){
	Node* finder = head;
		
	while (finder!=NULL){
		if(finder->isVoltageSetNode())
			finder = finder->getNext();
		else{
			finder->changeVoltage(((finder->firstTerm())*(secondTerm(finder->getNodeIndex()))));
			finder = finder->getNext();
		}
	}
	return;			
}

void NodeList::printSolveNode(){
	cout<<"Solve:"<<endl;
	Node* finder = head;
	
	while(finder!=NULL){
		finder->printSolvePerNode();
		finder = finder->getNext();
	}
}
	
	   		
