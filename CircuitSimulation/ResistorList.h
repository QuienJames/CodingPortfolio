#ifndef RESISTORLIST_H
#define RESISTORLIST_H

#include "Resistor.h"
using namespace std; 

class ResistorList
{
private:
	Resistor* head;
public:
	ResistorList();
	~ResistorList();
	
	void deleteResistor(string resistorName);
	void deleteAllResistors();
	void insertResistor(Resistor* newptr);
	void modifyResistor(string resistorName, double resistance_);
	bool findResistor (string resistorName);
	void printResistor (string resistorName);
	void printAllResistors()const;
	double returnResistanceResList(string resistorName);
	Resistor* returnHead()const;
	int returnNumRes()const;
	bool isHeadThere()const;
	double firstTermList();
	string returnNthResistorList(int n);
	//Various Setters Getters and Mutators
	
};

#endif	
