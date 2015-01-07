#include "Resistor.h"

 Resistor::Resistor(){
	name = "";
	resistance = 0;
	endpointNodeIDs[0] = 0;
	endpointNodeIDs[1] = 0;
	next = NULL;
}
 
 Resistor::~Resistor(){}
 
 void Resistor::setResistor(string name_, double resistance_, int endpoint1_, int endpoint2_, Resistor* n){
	name = name_;
	resistance = resistance_;
	endpointNodeIDs[0] = endpoint1_;
	endpointNodeIDs[1] = endpoint2_;
	next = n;
}
	
 Resistor::Resistor(string name_, double resistance_, int endpoint1_, int endpoint2_){
	name = name_;
	resistance = resistance_;
	endpointNodeIDs[0] = endpoint1_;
	endpointNodeIDs[1] = endpoint2_;
	next = NULL;
}
 	
 Resistor::Resistor(string name_,double resistance_,int endpoints_[2]){
	name = name_;
	resistance = resistance_;
	endpointNodeIDs[0] = endpoints_[0];
	endpointNodeIDs[1] = endpoints_[1];
}
   // rIndex_ is the index of this resistor in the resistor array
   // endpoints_ holds the node indices to which this resistor connects
	
string Resistor::getName() const{
   	return name;
}
    // returns the name
double Resistor::getResistance() const{
    return resistance;
}// returns the resistance

void Resistor::setResistance (double resistance_){
   	resistance = resistance_;
}
   
Resistor* Resistor::getNext() const{
   	return next;
}
   
void Resistor::setNext(Resistor* n){
	next = n;
}
      

   	
   // printing function essentially a modular function for double functions
void Resistor::print()const{
	int spaces = (20 - name.size());
	cout<<name;
	for(int i=1;i<=spaces;i++)
	cout<<" ";
	   	
	if (resistance<=9)
	cout<<"    ";
	else if(resistance<=99)
	cout<<"   ";
   	else if(resistance<=999)
   	cout<<"  "; 
   	else if(resistance<=9999)
   	cout<<" ";
   	else;
   	
   	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);
	cout<<resistance<<" Ohms "<<endpointNodeIDs[0]<<" -> "<<endpointNodeIDs[1]<<endl;	
}	

int Resistor::getOtherNodeID(int currentNodeID){
	if(currentNodeID==endpointNodeIDs[1])
		return endpointNodeIDs[2];
	else
		return endpointNodeIDs[1];
}

   	
