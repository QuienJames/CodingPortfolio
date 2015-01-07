/* 
 * File:   Node.h
 * Author: JC and VB
 *
 * Created on October 6, 2013, 12:58 PM
 *Editted by: James Anthony Quien
 */
 
#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "Resistor.h"
#include "ResistorList.h"

using namespace std; 

class Node
{
private:
	
   int nIndex;
   double voltage;
   double oldVoltage;
   bool isVoltageSet;
   Node* next;
   ResistorList resistorList;
   
public:
	//initialize classes and begins "empty" setup
   Node();
   Node(int nIndex_);
   Node(int nIndex_,Node *n);
   ~Node();
  
   void setNodeIndex (int nodeIndex);
   void setNext (Node *n);
   //setters
   
   bool findAndSetResistor (string resistorName_, double newResistance_);
   bool findResistor (string resistorName_);
   //getters and permutated getters
   
   double returnResistance (string resistorName_);
   void deleteResistor (string resistorName_);
   void deleteAllResistors();
	//deleters
   int getNodeIndex() const;
   double getVoltage() const;
   bool checkSetVoltage() const;
   bool getResList();
   Node* getNext() const;
   //More Getters
   
   void setV(double voltage_);
   void unset();
   void solve();
   bool isVoltageSetNode();
   double getDeltaV();
   double firstTerm();
   ResistorList retrieveResList()const;
   void changeVoltage(double voltage_);
   void printSolvePerNode();
   int returnNumResNode();
   string returnNthResistor(int n);

   //"Set Voltage" Functions
   
   void addResistor(Resistor* newRes);
   
   void print ()const; 
   void printRes(string resistorName);
   //prints each 

};

#endif	/* NODE_H */
