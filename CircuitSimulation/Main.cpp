//JAMES ANTHONY DY QUIEN 999777308 
//RESISTOR AND NODE 

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

#include "Rparser.h"
#include "NodeList.h"
#include "ResistorList.h"
#include "Node.h"
#include "Resistor.h"

using namespace std;

int main (void)
{
	string line, command;
	NodeList* nodeList = new NodeList();
	
	cout<< "> ";
 	getline(cin,line);
 	
 	//MAKE SURE TO FIND IF THE NODES EXIST, CREATE IT IF THEY DON'T 
	 //AND INSERT THE PROPER RESISTORS, TWICE IF YOU HAVE TO
 while (!cin.eof()){
 	RParser* Parsing = new RParser(); 

    stringstream lineStream (line);
 	lineStream>>command;
 	
 	//functions
	if (lineStream.fail())
		cout<<"Error: invalid command"<<endl;
	
    	else if (command == "insertR"&&Parsing->insert_R(lineStream)){
		 	int nodeid1 = Parsing->getNodeid1();
		 	int nodeid2 = Parsing->getNodeid2();
		 	//if node is not present, it is created
		 	if(!nodeList->findNode(nodeid1)){
		 		nodeList->insertNode(new Node(nodeid1));
		 	}
		 	
		 	if(!nodeList->findNode(nodeid2)){
		 		nodeList->insertNode(new Node(nodeid2));
		 	} //Two nodes created if not already existing
		 	
		 	nodeList->addResistor(nodeid1, new Resistor(Parsing->getName(),Parsing->getResistance(),nodeid1,nodeid2));
		 	nodeList->addResistor(nodeid2, new Resistor(Parsing->getName(),Parsing->getResistance(),nodeid1,nodeid2));
		 	//insert Two Resistors
		 	cout<<"Inserted: resistor "<<Parsing->getName()<<" ";
		   	cout.setf(ios::fixed);
			cout.setf(ios::showpoint);
			cout.precision(2);
			cout<<Parsing->getResistance()<<" Ohms "<<nodeid1<<" -> "<<nodeid2<<endl;
			//printed for valid insert
		}
	
   		 else if (command == "modifyR"&&Parsing->modify_R(lineStream)){
	 		 
	 		 if(nodeList->findResistor(Parsing->getName())){
	 		 	
		 		 double oldResistance = nodeList->returnOldResistance(Parsing->getName());
		 		 
		 		 if(nodeList->findAndSetResistorList(Parsing->getName(), Parsing->getResistance()))
		 			 cout<<"Modified: resistor "<<Parsing->getName()<<" from "<<oldResistance<<" Ohms to "<<Parsing->getResistance()<<" Ohms"<<endl;
			 } //Function executed else an error occurs
			 
			 else
				cout<<"Error: resistor "<<Parsing->getName()<<" not found"<<endl;	
			//See if resistor exists or not
		}
 			 
  	  else if (command == "printR"&&Parsing->print_R(lineStream)){
			if(nodeList->findResistor(Parsing->getName()))
				nodeList->printResistor(Parsing->getName());
				//PRINT THIS ONE RESISTOR IF IT EXISTS
			else
				cout<<"Error: resistor "<<Parsing->getName()<<" not found"<<endl;
		}
	
  	  else if (command == "printNode"&&Parsing->print_Node(lineStream)){
			//prints all the nodes or each node
			if(Parsing->getName()=="all")
				nodeList->printAllNodes();
			else if(nodeList->findNode(Parsing->getNodeid1()))
				nodeList->printNode(Parsing->getNodeid1());
			else
				cout<<"Error: node "<<Parsing->getNodeid1()<<" not found"<<endl;
		}
	
		else if (command == "deleteR"&&Parsing->delete_R(lineStream)){
			if (Parsing->getName()=="all"){
				nodeList->deleteAllResistors();
				cout<<"Deleted: all resistors"<<endl;
			} //delete all resistors
	 		else if(nodeList->findResistor(Parsing->getName())){
	 			nodeList->deleteResistor(Parsing->getName());	
				cout<<"Deleted: resistor "<<Parsing->getName()<<endl;
				}	//DELETE RESISTOR FOR THE TWO NODES
			else
				cout<<"Error: resistor "<<Parsing->getName()<<" not found"<<endl;
			}		
 		
 		else if(command == "setV"){
 			int nodeid;
			double voltage;
 			lineStream>>nodeid;
			lineStream>>voltage;
			
			if(!nodeList->findNode(nodeid)){
		 		nodeList->insertNode(new Node(nodeid));
		 	}
			
 			nodeList->setV(voltage,nodeid);
 			cout<<"Set: node "<<nodeid<<" to ";
		   	cout.setf(ios::fixed);
			cout.setf(ios::showpoint);
			cout.precision(2);
			cout<<voltage<<" Volts"<<endl;
 			}
 			
 		else if(command == "unsetV"){
 			int nodeid;
 			lineStream>>nodeid;
 			
 			nodeList->unset(nodeid);
 			cout<<"Unset: the solver will determine the voltage of node "<<nodeid<<endl;
 			}
 			
 		else if(command == "solve"){
 			if(nodeList->solve())
 			nodeList->printSolveNode();
 			}
 			
 		else
    		cout<< "Error: invalid command" << endl;
    
   		 cout<< "> ";
 		getline(cin,line);
		delete Parsing;
 	}
 delete nodeList;
 //Clean up everything; deleting all dynamically allocated arrays
return 0;
}
