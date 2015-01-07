//Header file Parser.h
//James Anthony Dy Quien 999777308 quienjam james.quien@mail.utoronto.ca
#ifndef RPARSER_H
#define RPARSER_H

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std; 

class RParser
{
	string name, toomany, nodeidall;
	double resistance, power;
	int nodeid,nodeid1, nodeid2; 		
	char e;
		
public:
	RParser();
	~RParser();
	bool insert_R(stringstream &lineStream);
	bool modify_R(stringstream &lineStream);
	bool print_R(stringstream &lineStream);
	bool delete_R(stringstream &lineStream);
	bool print_Node(stringstream &lineStream);
	//Various setters
		
	string getName();
	double getResistance();
	int getNodeid1();
	int getNodeid2();
	//getters
};
#endif

		
		
