#include "Node.h"
#include "Resistor.h"
#include "ResistorList.h"

Node::Node(){
	nIndex = 0;
	voltage = 0;
	oldVoltage = 0;
	isVoltageSet = false;
	next = NULL;
}

Node::Node(int nIndex_){
  	nIndex = nIndex_;
	voltage = 0;
	oldVoltage = 0;
	isVoltageSet = false;
	next = NULL;
}
   
Node::Node(int nIndex_,Node *n){
   	nIndex = nIndex_;
	voltage = 0;
	oldVoltage = 0;
	isVoltageSet = false;
	next = n;
}

Node::~Node(){
    resistorList.deleteAllResistors();
}
   	
   
   // Updates resIDArray to show the resistor in position rIndex in
   // the resistor array is now connected to this node.
   // Returns true if successful
   
void Node::setNodeIndex (int nodeIndex){
 	nIndex=nodeIndex;
}

void Node::setNext (Node* n){
   	next = n;
}
   	
bool Node::findAndSetResistor (string resistorName_, double newResistance_){
 	if(!resistorList.findResistor(resistorName_))
   		return false;
   	else {
   		resistorList.modifyResistor(resistorName_, newResistance_);
   		return true;
   		}
}
   	
bool Node::findResistor (string resistorName_){
   	if(!resistorList.findResistor(resistorName_))
   		return false;
   	else 
   		return true;
}
   	
int Node::getNodeIndex() const{
   	return nIndex;
}
   		
double Node::getVoltage() const{
   	return voltage;
}
   	
bool Node::checkSetVoltage() const{
   	return isVoltageSet;
}
   	
Node* Node::getNext() const{
  	return next;
}
   	
void Node::print ()const{
	if(!resistorList.isHeadThere())
   		return;
   	else{
   		cout<<"Connections at node "<<nIndex<<": "<<resistorList.returnNumRes()<<" resistor(s)"<<endl;
   		resistorList.printAllResistors();
   	}
}
   
int Node::returnNumResNode(){
   	return resistorList.returnNumRes();
}
   	
string Node::returnNthResistor(int n){
   	return resistorList.returnNthResistorList(n);
}

   
void Node::printRes(string resistorName){
   	resistorList.printResistor(resistorName);
}
   	
void Node::deleteResistor (string resistorName_){
	 resistorList.deleteResistor(resistorName_);
}
		
void Node::addResistor(Resistor* newRes){
   	resistorList.insertResistor(newRes);
}

double Node::returnResistance (string resistorName_){
   	return resistorList.returnResistanceResList(resistorName_);
}

void Node::deleteAllResistors(){
   	resistorList.deleteAllResistors();
}	

bool Node::getResList(){
   	if (resistorList.isHeadThere())
   		return true;
   	else
   		return false;
}	
   	
ResistorList Node::retrieveResList()const{
  	return resistorList;
}

void Node::setV(double voltage_){
   	isVoltageSet = true;
   	oldVoltage = voltage;
   	voltage = voltage_;
}
   	
void Node::changeVoltage(double voltage_){
   	oldVoltage = voltage;
   	voltage = voltage_;
}
   	
void Node::unset(){
   	isVoltageSet = false;
  	voltage = 0;
}
   	   
bool Node::isVoltageSetNode(){
	return isVoltageSet;
}
   	
double Node::getDeltaV(){
   	double i;
   	i = oldVoltage - voltage;
   	if(i<0)
   		i = i*(-1);
 	return i;
}

double Node::firstTerm(){
	return resistorList.firstTermList();
}
	
void Node::printSolvePerNode(){
	cout<<"  Node "<<nIndex<<": ";
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);
	cout<<voltage<<" V"<<endl;
}
