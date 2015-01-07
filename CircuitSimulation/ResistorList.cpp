#include "ResistorList.h"
#include "Resistor.h"

ResistorList::ResistorList(){
	head = NULL;
}

ResistorList::~ResistorList(){
	while (head!=NULL){
		Resistor* temp = head;
		head = head->getNext();
		delete temp;
	}	
}
	
void ResistorList::deleteResistor(string resistorName){
	Resistor* current = head;
	Resistor* previous = NULL;
	
	if (head ==NULL)
	return;
	
	while (current!=NULL&&current->getName()!=resistorName){
		previous = current;
		current = current -> getNext();
	}
	
	if(current==NULL)
		return;
	
	if(previous==NULL)
		head = head->getNext();
	else 
		previous->setNext(current->getNext());
		
	delete current;
	return;
}
	

void ResistorList::deleteAllResistors(){
	while (head!=NULL){
		Resistor* temp = head;
		head = head->getNext();
		delete temp;
	}
}

void ResistorList::insertResistor(Resistor* newPtr){
	if(head==NULL)
		head = newPtr;
	else{
		Resistor* tail = head;
		
		while (tail->getNext()!=NULL)
			tail = tail->getNext();
			
		tail->setNext(newPtr);
	}
	return;
}

void ResistorList::modifyResistor(string resistorName, double resistance_){
	Resistor* finder = head;
	
	while (finder!=NULL&&finder->getName()!=resistorName)
		finder = finder->getNext();
	
	if (finder== NULL)
		return;
		
	else if (finder->getName()==resistorName){
		finder->setResistance(resistance_);
		return;
	}
	return;
}

bool ResistorList::findResistor (string resistorName){
	Resistor* finder = head;
	
	while (finder!=NULL&&finder->getName()!=resistorName)
		finder = finder->getNext();
	
	if (finder== NULL)
		return false;
	else if (finder->getName()==resistorName)
		return true;
	else
	return false;
}
	
void ResistorList::printResistor (string resistorName){
	Resistor* finder = head;
	
	while (finder!=NULL&&finder->getName()!=resistorName)
		finder = finder->getNext();
		
	 if (finder!=NULL){
	 	cout<<"Print:"<<endl;
		finder->print();
		}
	return;
}

double ResistorList::firstTermList(){
	double firstTerm, denominator = 0;
	
	Resistor* finder = head;
	
	while (finder!=NULL){
		denominator = denominator + (1/finder->getResistance());
		finder = finder->getNext();
	}
		
	firstTerm = 1/(denominator);
	return firstTerm;
}

void ResistorList::printAllResistors()const{
	Resistor* temp = head;
	
	if (temp==NULL)
		return;

	while (temp!=NULL){
		cout<<"  ";
		temp->print();
		temp = temp -> getNext();
	}
	
	return;
}

double ResistorList::returnResistanceResList(string resistorName){
	Resistor* finder = head;
	
	while (finder!=NULL){
		if(finder->getName()==resistorName)
			return finder->getResistance();
		else
			finder = finder->getNext();
		}
	return 0;	
}

Resistor* ResistorList::returnHead()const{
	return head;
}

int ResistorList::returnNumRes()const{
	int resNum = 0;
	Resistor* finder = head;
	
while (finder!=NULL){
	resNum++;
	finder = finder->getNext();
	}
	
	return resNum;
}
	
bool ResistorList::isHeadThere()const{
	if (head==NULL)
		return false;
	else
		return true;
}

string ResistorList::returnNthResistorList(int n){
	Resistor* finder = head;
	int place = 1;
	
	while(finder!=NULL&&place!=n){
		finder = finder->getNext();
		place++;
	}
		return finder->getName();
}

		

