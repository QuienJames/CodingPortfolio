#ifndef NODELIST_H
#define NODELIST_H

#include "Node.h"
#include "ResistorList.h"
using namespace std; 

class NodeList
{
private:
	Node* head;
public:
	NodeList();
	~NodeList();
	
	void insertNode(Node* newPtr);
	bool findNode (int nodeID_);
	bool printNode (int nodeID_);
	void printAllNodes();
	
	void addResistor(int nodeID_,Resistor* newRes);
	bool findAndSetResistorList (string resistorName_, double newResistance_);
	bool findResistor (string resistorName_);
	void deleteResistor (string resistorName_);
	void deleteAllResistors();
	void printResistor (string resistorName_);
	double returnOldResistance (string resistorName_);
	//Various Getters and Setters and Mutators
	
	void setV(double voltage_,int nodeID_);
    void unset(int nodeID_);
    bool solve();
    bool checkAnyVoltageSet();
    bool someGreaterThanMinItChange(); //function to tell whether or not to stop changing voltages for each node
    string findCommonRes(Node* finder_,Node* current_);//finds a common resistor between nodes
    double secondTerm(const int currentNodeID);
    void newVoltageValue();//Changes the voltage for unset nodes
    void printSolveNode();
    //"Voltage Solving" functions
    //A combination of these functions, associated functions and other functions within NodeList are utilized in the solving process
};

#endif	
