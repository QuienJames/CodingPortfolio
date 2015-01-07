/* 
 * File:   Resistor.h
 * Author: JC and VB
 *
 * Created on October 6, 2013, 1:01 PM
 */

#ifndef RESISTOR_H
#define RESISTOR_H

#include <string>
#include <iostream>
#include <cmath>
using namespace std;

class Resistor
{
private:
   double resistance; // resistance (in Ohms)
   string name; // C++ string holding the label
   int endpointNodeIDs[2]; // IDs of nodes it attaches to
   Resistor* next;
   
public:
   Resistor();
   Resistor(string name_,double resistance_,int endpoints_[2]);
   Resistor(string name_, double resistance_, int endpoint1_, int endpoint2_);

   // rIndex_ is the index of this resistor in the resistor array
   // endpoints_ holds the node indices to which this resistor connects

   ~Resistor();
   void setResistor(string name_, double resistance_, int endpoint1_, int endpoint2_, Resistor* n);
   string getName() const; // returns the name
   double getResistance() const; // returns the resistance
   int getOtherNodeID(int currentNodeID);

   void setResistance (double resistance_);
   void setNext(Resistor* n);
   
   Resistor* getNext() const;

   void print ()const;
};

#endif	/* RESISTOR_H */
