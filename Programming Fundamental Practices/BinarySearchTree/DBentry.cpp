#include "DBentry.h"

//ALL PUBLIC FUNCTIONS SAVE FOR THE FINDMAX
DBentry::DBentry(){
//	cout<<"DB constructed"<<endl;
}

DBentry::DBentry(string _name, unsigned int _IPaddress, bool _active){
	name = _name;
	IPaddress = _IPaddress;
	active = _active;
	//	cout<<"DB constructed"<<endl;
}

bool DBentry::getActive() const{
	return active;
}

unsigned int DBentry::getIPaddress() const{
	return IPaddress;
}

string DBentry::getName() const{
	return name;
}

void DBentry::setActive(bool _active){
	active = _active;
}

void DBentry::setIPaddress(unsigned int _IPaddress){
	IPaddress = _IPaddress;
}

void DBentry::setName(string _name){
	_name = name;
}

DBentry::~DBentry(){
	//	cout<<"DB destructed"<<endl;
}
	
	
	
