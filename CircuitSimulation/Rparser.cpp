#include "Rparser.h"

RParser::RParser(){}
RParser::~RParser(){}
string RParser::getName()
{
	return name;
}

double RParser::getResistance()
{
	return resistance;
}

int RParser::getNodeid1()
{
	return nodeid1;
}

int RParser::getNodeid2()
{
	return nodeid2;
}

bool RParser::insert_R (stringstream &lineStream)
	{
 		lineStream>>name;
 		
		if (lineStream.fail())//check whether or not it failed
 		{
	 		if (lineStream.eof())
	 		{
	 			cout<<"Error: too few arguments"<<endl;
	 			return 0;
	 		}
	 		else
	 		{
	 		 	cout<<"Error: invalid argument"<<endl;
	 		 	return 0;
	 		}
	 		
 		}
 		else if(name=="all")//check for all
 		{
 			cout<<"Error: resistor name cannot be the keyword \"all\""<<endl;
 			return 0;
		}
		else if(lineStream.peek()!=' '&&lineStream.peek()!='\t'&&lineStream.eof()!=1)//check for invalid input
 		{
 			cout<<"Error: invalid argument"<<endl;
 			return 0;
 		}
 		
 		lineStream>>resistance;
 		
 		if (lineStream.fail())
 		{
	 		if (lineStream.eof())
	 		{
	 			cout<<"Error: too few arguments"<<endl;
	 			return 0;
	 		}
	 		else
	 		{
	 		 	cout<<"Error: invalid argument"<<endl;
	 		 	return 0;
	 		}
 		}
 		else if(lineStream.peek()=='e'||lineStream.peek()=='E')//check for input in scientific notation
 		{
 			lineStream>>e;
 			lineStream>>power;
 			resistance = resistance*(pow(10,power));
 		}
		else if(lineStream.peek()!=' '&&lineStream.peek()!='\t'&&lineStream.eof()!=1)//check invalid input
 		{
 			cout<<"Error: invalid argument"<<endl;
 			return 0;
 		}
 		else if(resistance<0)
 		{
 			cout<<"Error: negative resistance"<<endl;
 			return 0;
 		}
 		
 		
 		lineStream>>nodeid1;
 		
 		if (lineStream.fail()||lineStream.peek()=='.')//check if fail
 		{
	 		if (lineStream.eof())
	 		{
	 			cout<<"Error: too few arguments"<<endl;
	 			return 0;
	 		}
	 		else
	 		{
	 		 	cout<<"Error: invalid argument"<<endl;
	 		 	return 0;
	 		}
 		}
		else if(lineStream.peek()!=' '&&lineStream.peek()!='\t'&&lineStream.eof()!=1)//check for invalid input
 		{
 			cout<<"Error: invalid argument"<<endl;
 			return 0;
 		}
 		
 		lineStream>>nodeid2;
 		
 		if (lineStream.fail()||lineStream.peek()=='.')//check if fail
 		{
	 		if (lineStream.eof())
	 		{
	 			cout<<"Error: too few arguments"<<endl;
	 			return 0;
	 		}
	 		else
	 		{
	 		 	cout<<"Error: invalid argument"<<endl;
	 		 	return 0;
	 		}
 		}
		else if(lineStream.peek()!=' '&&lineStream.peek()!='\t'&&lineStream.eof()!=1)//check for invalid input
 		{
 			cout<<"Error: invalid argument"<<endl;
 			return 0;
 		}
		else if(nodeid1==nodeid2)
		{
			cout<<"Error: both terminals of resistor connect to node "<<nodeid1<<endl;
			return 0;
		}
		
		
		lineStream>>toomany;//check for too many arguments
		
		if(lineStream.fail()&&lineStream.eof())
		{	
			return 1;
		}
		else
		{
		    cout<<"Error: too many arguments"<<endl;
	     	return 0;
		}
	}
	
	bool RParser::modify_R (stringstream &lineStream)
	{

		lineStream>>name;
 		
		if (lineStream.fail()) //check for invalid input or too few arguments
 		{
	 		if (lineStream.eof())
	 		{
	 			cout<<"Error: too few arguments"<<endl;
	 			return 0;
	 		}
	 		else
	 		{
	 		 	cout<<"Error: invalid argument"<<endl;
	 		 	return 0;
	 		}
 		}
		else if(lineStream.peek()!=' '&&lineStream.peek()!='\t'&&lineStream.eof()!=1)//check for invalid input
 		{
 			cout<<"Error: invalid argument"<<endl;
 			return 0;
 		}
 		else if(name=="all")
 		{
 			cout<<"Error: resistor name cannot be the keyword \"all\""<<endl;
 			return 0;
		}
 		
 		
 		lineStream>>resistance;
 		
 		if (lineStream.fail())//check for error
 		{
	 		if (lineStream.eof())
	 		{
	 			cout<<"Error: too few arguments"<<endl;
	 			return 0;
	 		}
	 		else
	 		{
	 		 	cout<<"Error: invalid argument"<<endl;
	 		 	return 0;
	 		}
 		}
		else if(lineStream.peek()!=' '&&lineStream.peek()!='\t'&&lineStream.eof()!=1)
 		{
 			cout<<"Error: invalid argument"<<endl;
 			return 0;
 		}
 		else if(lineStream.peek()=='e'||lineStream.peek()=='E')//check for scientific notation
 		{
 			lineStream>>e;
 			lineStream>>power;
 			resistance = resistance*(pow(10,power));
 		}
 		else if(resistance<0)
 		{
 			cout<<"Error: negative resistance"<<endl;
 			return 0;
 		}
 		
 		
		lineStream>>toomany;
		
		if(lineStream.fail()&&lineStream.eof())//output
		{
			return 1;
		}
		else
		{
		cout<<"Error: too many arguments"<<endl;
		return 0;
		}
	}
	
	bool RParser::print_R (stringstream &lineStream)
	{
 		lineStream >> name;
 		if(lineStream.fail())//input
 		{
			if (lineStream.eof())
	 		{
	 			cout<<"Error: too few arguments"<<endl;
	 			return 0;
	 		}
	    	else
	    	{
	    	cout<<"Error: invalid argument"<<endl;
	    	return 0;
	    	}
	    }
 		else if(lineStream.peek()!=' '&&lineStream.peek()!='\t'&&lineStream.eof()!=1)
 		{
 			cout<<"Error: invalid argument"<<endl;
 			return 0;
 		}
 		
	    lineStream>>toomany;//check for too many arguments
	    
	    if (lineStream.fail()&&lineStream.eof())
	    {
	 			if (name=="all")
	 			return 1;
	 			else
	 			return 1;
    	}
    	else
    	{
		cout<<"Error: too many arguments"<<endl;
		return 0;
		}
    }
    
	bool RParser::delete_R (stringstream &lineStream)
	{
 		lineStream >> name;
 		if(lineStream.fail())
 		{
			if (lineStream.eof())
	 		{
	 			cout<<"Error: too few arguments"<<endl;
	 			return 0;
	 		}
	    	else
	    	{
	    	cout<<"Error: invalid argument"<<endl;
	    	return 0;
	    	}
	    }
 		else if(lineStream.peek()!=' '&&lineStream.peek()!='\t'&&lineStream.eof()!=1)//check for invalid input
 		{
 			cout<<"Error: invalid argument"<<endl;
 			return 0;
 		}
 		
	    lineStream>>toomany;
	    
	    if (lineStream.fail()&&lineStream.eof())
	    {
	 			if (name=="all")
	 			{
	 			return 1;
	 			}
	 			else
	 			{
	 			return 1;
	 			}
    	}
    	else
    	{
		cout<<"Error: too many arguments"<<endl;
		return 0;
		}
    }

	bool RParser::print_Node(stringstream &lineStream)
	{
 		
 		lineStream>>name;
 		
 		if(lineStream.fail())//check if general error
 		{
			if (lineStream.eof())
	 		{
	 			cout<<"Error: too few arguments"<<endl;
	 			return 0;
	 		}
	    	else
	    	{
		    	cout<<"Error: invalid argument"<<endl;
		    	return 0;
	    	}
	    }
 		else if(lineStream.peek()!=' '&&lineStream.peek()!='\t'&&lineStream.eof()!=1)//check for invalid input
 		{
 			cout<<"Error: invalid argument"<<endl;
 			return 0;
 		}
 		
		lineStream>>toomany;
		 		
	    
		 if (name=="all")
		{
	    	return 1;
	   	}
	    else 
	    {
	    	stringstream carrier(name);
		    carrier>>nodeid1;
	 		if (carrier.fail()||carrier.peek()=='.')
	 		{
		 		if (carrier.eof())//check for general error
		 		{
		 			cout<<"Error: too few arguments"<<endl;
		 			return 0;
		 		}
		 		else
		 		{
		 		 	cout<<"Error: invalid argument"<<endl;
		 		 	return 0;
		 		}
	 		}
			else if(carrier.peek()!=' '&&carrier.peek()!='\t'&&carrier.eof()!=1)//check for invalid input
 	            {
 		        	cout<<"Error: invalid argument"<<endl;
 		        	return 0;
 		        }
		 	else if (lineStream.fail()!=1)
		{
			cout<<"Error: too many arguments"<<endl;
			return 0;
		}
		 	else
		 	{
			 	return 1;
			}
		}
	}
