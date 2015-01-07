#include <iostream>
#include <sstream>
#include <string>

#include "TreeDB.h"
#include "TreeNode.h"
#include "DBentry.h"

using namespace std;

int main (void)
{
	string line, command, name, statusName;
	unsigned int IPaddress;
	bool status;
	
	TreeDB* treeDataBase = new TreeDB();

	cout<< "> ";
 	getline(cin,line);
 	//check for end of file
	while (!cin.eof()){		
		stringstream lineStream (line);
		lineStream>>command;
		
		//functions
		//Assuming no syntax errors and produces output accordingly
		if (lineStream.fail())
			cout<<"Error: invalid command"<<endl;		
 		else if (command == "insert"){
 			lineStream>>name;
 			lineStream>>IPaddress;
 			lineStream>>statusName;
 			
 			if(statusName == "active")
 				status = true;
 			else
 				status = false;
 			
 			if(treeDataBase->insert(new DBentry(name, IPaddress, status)))
 				cout<<"Success"<<endl;
 			else
 				cout<<"Error: entry already exists"<<endl;
 		}
 		else if (command == "find"){
 			lineStream>>name;
 			
 				if(treeDataBase->find(name) == NULL)
 					cout<<"Error: entry does not exist"<<endl;
 				else{
 					cout<<treeDataBase->find(name)->getName()<<" : "<<treeDataBase->find(name)->getIPaddress()<<" : ";
					 if(treeDataBase->find(name)->getActive())
					 	cout<<"active"<<endl;
					 else
					 	cout<<"inactive"<<endl;
				}
		}
		else if (command == "remove"){
			lineStream>>name;
			
			if(treeDataBase->remove(name))
				cout<<"Success"<<endl;
			else
				cout<<"Error: entry does not exist"<<endl;
		}
		else if (command == "printall"){
			treeDataBase->printAll();
		}
		else if (command == "printprobes"){
			lineStream>>name;
			
			if(treeDataBase->find(name)==NULL)
				cout<<"Error: entry does not exist"<<endl;
			else
				treeDataBase->printProbes();
		}
		else if (command == "removeall"){
			treeDataBase->clear();
			cout<<"Success"<<endl;
		}
		else if (command == "countactive"){
			treeDataBase->countActive();
		}
		else if (command == "updatestatus"){
			lineStream>>name;
			lineStream>>statusName;
			
 			if(statusName == "active")
 				status = true;
 			else
 				status = false;
			
			treeDataBase->updateStatus(name, status);
		}
		else
    		cout<< "Error: invalid command" << endl;
    
   		 cout<< "> ";
 		getline(cin,line);
	}
	//clear database
	delete treeDataBase;
	return 0;
}
