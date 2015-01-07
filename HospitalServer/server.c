/**
  * @file
 * @brief This file access the database and manipulates it with the help of BST
 *
 * The has become very long and useless, it does everything it is asked to do, but it will only get longer as more functions are implemented
 *
 * The storage server communicates with the library functions declared in storage.h and implemented in storage.c.
 */
#include "BST.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include "utils.h"
#include "BST.h"
#include <stdbool.h>
#include <ctype.h>

#define MAX_LISTENQUEUELEN 20	///< The maximum number of queued connections.
FILE*server_file;
FILE * file;
FILE *sorted_file;
int extern global_logger;
int handle_command(int sock, char *cmd);
struct config_params params;
int cA (char *cmd,char *user, char*pass);
int gV(char *cmd,char *newValue[]);
int sV(char *cmd);
int query(char*cmd, char keys[1000][20]);
int dV (char *cmd);
int setData (char *cmd,char name[MAX_KEY_LEN], char values [MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE],node **subN);
int Auth=0; int tableNum=0;
void BST();
/**
 * @brief Process a command from the client.
 *
 * @param sock The socket connected to the client.
 * @param cmd The command received from the client.
 * @return Returns 0 on success, a negative value that determines the type of error otherwise.
 */
//valgrind --tool=memcheck --show-reachable=yes --leak-check=full server default.conf
int handle_command(int sock, char *cmd)
{
	int cPass=1;
	int status;
	int i=0;
	char returnKeys[50][20];
	char *newValue[MAX_COLUMNS_PER_TABLE];
	char jamestown[sizeof(cmd)];
	memset (jamestown,0,sizeof jamestown);

	printf ("the cmd is %s \n",cmd);
    {//LOGGER
		char command[80];
		strcpy(command, "Command: ");
		logger(server_file,strcat(command,cmd));
		logger(server_file,"\n");
    }
	if (cmd[0]=='A'){ //if the command is Authenticate call the function
		cPass = cA(cmd,params.username,params.password); // cPass checks to see if it passes
		if (cPass>=0){ Auth=1;strcpy(jamestown,"Successful\n");} // puts successful into jamestown, no real reason
	}
	else if (cmd[0]=='G'){ // Get call check
		cPass = gV(cmd,newValue);
		if (cPass<0)
			printf ("%d  \n",cPass);
		else{
			int i=0;
			for (i=0;i<params.tableColCount[tableNum];i++)
			{
				strcat (jamestown,params.tableCol[tableNum][i]); // assigns the value of the location, NULL if it DNE
				strcat (jamestown," ");
				strcat (jamestown,newValue[i]);
				if ((i+1)<params.tableColCount[tableNum])
					{strcat (jamestown,", ");}
			}
				strcat (jamestown,"\n");

		}
	}
	else if (cmd[0]=='S'){ //Set call check
		cPass = sV(cmd);
		if (cPass<0)
			printf ("%d  \n",cPass);
		else{
			snprintf (jamestown,800, "Success \n"); // assigns the value of the location, NULL if it DNE
			}
	}
	else if (cmd [0] == 'D') { //Disconnect call
		cPass=-5; //-5 Disconnect
	}
	else if (cmd [0] == 'L'){ //Load value call, not used in Milestone 2
		BST();
	}
	else if(cmd[0] == 'Q')
	{
		cPass = query(cmd, returnKeys);
		if (cPass<0)
			printf ("%d  \n",cPass);
	}
	else{cmd[0]='O';}		// -1 invalid username or password
			//-3 incorrect table name or key information
				//-4 incorrect key information
	if (cPass<0){//If an error occured
		snprintf (jamestown,10, "%d\n",cPass);// the error gets passed to jamestown and

	}
	if (cmd[0]=='O');
	else if(cmd[0]!='Q' &&cmd[0]!='O') // This might have some problems
		status = sendall(sock, jamestown, strlen(jamestown));// sent to the storage
	else if(cPass>=0)
	{
		snprintf(jamestown,800,"%d \n", cPass);
		status = sendall(sock, jamestown, strlen (jamestown))+recvline(sock,jamestown, sizeof(jamestown));
		i = 0;
		printf ("string1:%s\n",jamestown);
		while(i<1000 && returnKeys[i][0]!= 0)
		{
			snprintf(jamestown,800,"%s\n", returnKeys[i]);
			printf ("string2:%s\n",jamestown);
			status = sendall(sock, jamestown, strlen (jamestown))+recvline(sock,jamestown, sizeof (jamestown));
			i++;
			if(status!=0){printf("breaks\n");
				break;}
		}
		snprintf(jamestown,800,"\n");
		status = sendall(sock,jamestown, strlen(jamestown));
	}
	else{
		status = sendall(sock, jamestown, strlen(jamestown));
	}
	if (status!=0)
	{
		cPass=-6; //-6 send report failed
	}

	{
		char command[80];
		strcpy(command, "Reply: ");
		logger(server_file,strcat(command,jamestown));
		logger(server_file,"\n");
	}
	return cPass;
}
/**
 * @brief Uses the binary search tree to build a data storage
 *
 **/

void BST()
{
        int i=0,l=0;
	int k=0;
    	char buf[MAX_CONFIG_LINE_LEN];
        char name[MAX_KEY_LEN];
        char pop [MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE];//aka value
        int j=0;
    	file =fopen("censusClean.txt","r"); // text we use
    	if (!file)
        	return;
    	pop[0][1]=0;
    	pop[1][0]=0;
	for (;k<params.tableCount;k++)
	{
		strcpy(pop[0],params.table[k]);
		if (params.head[k]==NULL)
	   		params.head[k]=insert(params.head[k],"Llllllllllllll\0",pop);
		if (strcmp(params.table[k],"census")==0)
			l=k;
    }/*
	k=l;
	//if (l==-1)
		//params.head[k].left=NULL;
	while (fgets(buf,sizeof(buf), file)!=NULL) // reads all the infor in the file and turns it into a tree
        {
            while(buf[i] != ' ') // looks for a space which ends the key
            {
                name[i] = buf[i];
                i++;
            }
            name[i]=0;
            i++;
            while (buf[i] <='9' && buf[i]>='0') // looks for the end of the value
            {
                pop[j++]=buf[i++];
            }
            pop[j]=0;
            insert(params.head[k],name,pop); // puts it into the tree using insert
            memset(buf,0,strlen(buf));
            memset(name,0,strlen(name));
            memset(pop,0,strlen(pop));
            i = 0;
            j=0;
        }
        fclose(file); // closes the file afterward*/
    	return;
}
/**
 * @brief Uses the binary search tree to set things
 *
 **/
int sV (char *cmd)
{
	char name[MAX_KEY_LEN];
	char values [MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE];
	int error=0;
	node *subN=NULL;
	if (!Auth)
		return -7;
	error = setData (cmd,name,values,&subN);
	if (error<0)
		return error;
	if (set(subN, name, values)==NULL)
		return -4;
	return 0;
}
/**
 * @gV Uses the binary search tree to build a data storage
 *
 **/

int gV (char *cmd,char* newValue [MAX_COLUMNS_PER_TABLE]) // error when this is char * newValue[], it only goes up to 5 values
{
int i =0,j=0, found=0;
char table [MAX_TABLE_LEN];
char name [MAX_COLNAME_LEN];
node* subN=NULL;
	if (!Auth)
		return -7;
	while (*(cmd++)!=' ')
	{
	}
	while (*cmd!='&')
	{
		table[i++]=*(cmd++);
	}
	cmd++;
	table[i]=0;
	i=0;
	while (*(cmd+1)!=0)
	{
		name[i++]=*(cmd++);
	}
	name[i]=0;
	for (tableNum=0;tableNum<params.tableCount;tableNum++)
	{
		if (strcmp(table,params.table[tableNum])==0)
		{
			found=1;
			subN=params.head[tableNum];
			break;
		}
	}
	if (!found)
	{
		return -3;
	}
	if (subN==NULL)
	{
		char pop [MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE];
		pop[0][1]=0;
		pop[1][0]=0;
	   		params.head[tableNum]=insert(params.head[tableNum],"Llllllllllllll\0",pop);
	   		subN=params.head[tableNum];
	}
	subN=get (subN,name,newValue);
	if (NULL==subN)
		{return -4;}
	//strcpy(newValue,subN->population); rewrite this
	return 0;
}
int cA (char *cmd,char *user, char*pass)
{
	while (*cmd!=' ')
	{
		cmd++;
	}
	cmd++;
	while (*user!=0) //checks username
	{
	if (*(cmd++)!=*(user++))
		return -1;
	}
	cmd++;
	while (*pass!=0) //checks password
	{
		if (*(cmd++)!=*(pass++))
			return -1;
	}
	if (*cmd!=' ')
		return -1;
	return 0;
}


int query(char*cmd, char keys[50][20])
{
		int i=0;
		int j=0,k=0,p1=0,p2=0,found=0,notStr=0,max_keys=0,notInt=0;

		int numKeys=0;
		int clen [MAX_COLUMNS_PER_TABLE];
		node *subN=NULL;
		char symbol[MAX_COLUMNS_PER_TABLE];
		char table[MAX_TABLE_LEN];
		char columns[MAX_COLUMNS_PER_TABLE][MAX_COLNAME_LEN];
		char values[MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE+4];
		for (i=0;i<MAX_COLUMNS_PER_TABLE;i++)
		{
			memset (values[i],0,sizeof values[i]);
		}
		//possible errors - exceed max length or too many tables
		for (j=0;j<1;j++)
		{
			while ((*cmd!=' '||*(cmd+1)==' '))
			{
				cmd++;
			}
			cmd++;
		}
		i=0;
		while (*cmd!='&') // get the table name
		{
			table[i++]=*cmd;
			cmd++;
		}
		table [i]=0;
		cmd++;
		if (*cmd=='+') cmd++;
		while ((*cmd>='0'&&*cmd<='9'))
		{
			max_keys=10*max_keys+*cmd-48;
			cmd++;
		}
		cmd++;
		i=0;
		for (tableNum=0;tableNum<params.tableCount;tableNum++)//tries to find the table
		{
			if (strcmp(table,params.table[tableNum])==0)
			{
				found=1;
				subN=params.head[tableNum];
				break;
			}
		}
		if (!found)
		{
			return -3;
		}
		if (subN==NULL)
		{
			char pop [MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE];
			pop[0][1]=0;
			pop[1][0]=0;
		   		params.head[tableNum]=insert(params.head[tableNum],"Llllllllllllll\0",pop);
		   		subN=params.head[tableNum];
		}
		p1=0;
		if (cmd[p1]==0 || cmd[p1]=='\n'|| cmd[p1]=='&')
		{
			return -2;
		}

		for (k=0;k<params.tableColCount[tableNum];k++){
			j=0;
			p1=0;
			p2=0;
			values[i][1]=' ';
			values[i][3]=' ';
			while (cmd[p1]==' ')
			{p1++;printf ("cmd1:%c \n",cmd[p1]);} //while it is a space
			p2=p1;

			while (cmd[p2]!=0 &&cmd[p2]!='\n' && cmd[p2]!=' '&& cmd[p2]!='<'&& cmd[p2]!='>'&& cmd[p2]!='=')//find the end of the column
			{p2++;}
			if (cmd[p2]!=0 &&cmd[p2]!='\n'){
				if ((p2-p1+1)>=MAX_COLNAME_LEN)
				{
					printf ("column name is too long\n");
					return -2;
				}
				for (i=0;p1<p2;p1++)
				{
					columns[k][i]=cmd[p1];
					i++;
				}
			columns[k][i]=0;
			}
			printf ("column %d is '%s' \n",k, columns[k]);
			for (j=0;j<params.tableColCount[tableNum];j++){
				if(strcmp (columns[k],params.tableCol[tableNum][j])==0)
				{
					values[k][0]=(char)(j+48);
				}
			}
			if (values[k][0]==' ') // if the column is not found error
				return -2;

			cmd=cmd+p2;
			p1=0;
			while (cmd[p1]==' ')
				{p1++;
				printf ("cmd2:%c\n",cmd[p1]);}//while space
			if (cmd[p1]!='>'&&cmd[p1]!='<'&&cmd[p1]!='=')
				return -2;
			values[k][2]=cmd[p1];
			p1++;
			while (cmd[p1]==' ')
				{p1++;("cmd3:%c\n",cmd[p1]);}
			p2=p1;
			if (cmd[p1]==0 ||cmd[p1]=='\n') // if it finds the end of line before expected
				{return -2;} // too few inputs
			else
				{p2++;}// help avoid the case of cmd[p1] == '-' or '+' b/c these values are not accepted for string
			while (cmd[p2]!=0 && cmd[p2]!='\n' && cmd[p2]!=',')//find the end of the value
			{
				if (cmd[p2]!=' '&&!((cmd[p2]>='0'&&cmd[p2]<='9')||(cmd[p2]>='A'&&cmd[p2]<='Z')||(cmd[p2]>='a'&&cmd[p2]<='z')))
					{return -2;	} // check for values not=0-9 A-Z a-z ' '
				p2++;
			}
			i=p2-1; //starts looking on the char before so
			int subp1=p1; // saves the value of p1 here
			while (cmd[i]==' ')
			{i--;}//backtrack to find the first not ' ' with i as sub for p2
			clen[k]=0; // sets the length to 0
			if (params.tableColLength[tableNum][k]!=0) //if it's not an int
			{
				if ((i-p1+2)>=params.tableColLength[tableNum][k])
				{printf ("values is too long \n"); // set length is too long
					return -2;}
			}
			else if ((i-p1+2)>=MAX_STRTYPE_SIZE)
			{
				printf ("values2 is too long \n"); // set length is too long
					return -2;

			}
			notStr=0;
			notInt=0;
			if (cmd[p1]=='-'|| cmd[p1]=='+') // int if true
				{ notStr=1;}
			else if (cmd[p1]<'0' || cmd[p1]>'9' )
				{notInt=1;}
			values[k][4]=cmd[p1++]; // slight addition to compensate for starting j at 1
			for (j=1;p1<=i;p1++) // looks through the rest and stores it
			{
				values[k][j+4]=cmd[p1]; // stores the values
				if (cmd[p1]<'0' || cmd[p1]>'9' ) // not an int
				{	notInt=1;
					if (notStr)
						return -2;
					clen[k]=i+1-subp1; // change the column len b/c it's a string sets the string lengths
				}
				j++;
			}
			values[k][j+4]=0;
			printf ("value %d is '%s' \n",k, values[k]);
				cmd=cmd+p2;
			if (values[k][2]!= '=' && notInt)
				return -2;
				printf ("cmd2.5:'%c' \n",*cmd);
			if (*cmd==0 ||*cmd=='\n')
				break;
			cmd++; // in this case it's pointing to comma so skip it
			printf ("cmd4:%c \n",*cmd);
		}
		k++;
		printf ("cmd5:'%c' \n",*cmd);
		for (i=0;i<k;i++)
		{
			if( clen[i]> params.tableColLength[tableNum][values[i][0]-48])
				return -2;
		}
		if (k<MAX_COLUMNS_PER_TABLE){values[k][0]=0;}

		for (i=0;i<50;i++)
		{
			memset(keys[i],0,sizeof keys[i]);
		}

		BSTQuery(subN, values, max_keys, &numKeys, keys, k);
		printf ("numkeys %d K: %d MAX_KEYS: %d\n",numKeys, k, max_keys);
		for (i=0;i<numKeys;i++)
		{
			printf ("key%d :'%s'\n",i,keys[i]);
		}
return numKeys;
}









int setData (char *cmd,char name[MAX_KEY_LEN], char values [MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE],node **subN)
{
		int i=0;
		int j=0,k=0,p1=0,p2=0,found=0,notStr=0;
		int clen [MAX_COLUMNS_PER_TABLE];
		char table[MAX_TABLE_LEN];
		char columns[MAX_COLUMNS_PER_TABLE][MAX_COLNAME_LEN];
		//possible errors - exceed max length or too many tables
		for (j=0;j<1;j++)
		{
			while ((*cmd!=' '||*(cmd+1)==' '))
			{
				cmd++;
			}
			cmd++;
		}
		i=0;
		while (*cmd!='&') // get the table name
		{
			table[i++]=*cmd;
			cmd++;
		}
		table [i]=0;
		cmd++;
		i=0;
		while (*cmd!='&') // get the key name
		{
			name[i++]=*cmd;
			cmd++;
		}
		name[i]=0;
		cmd++;
		for (tableNum=0;tableNum<params.tableCount;tableNum++)//tries to find the table
		{
			if (strcmp(table,params.table[tableNum])==0)
			{
				found=1;
				*subN=params.head[tableNum];
				break;
			}
		}
		if (!found)
		{
			return -3;
		}
		if (*subN==NULL)
		{
			char pop [MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE];
			pop[0][1]=0;
			pop[1][0]=0;
		   		params.head[tableNum]=insert(params.head[tableNum],"Llllllllllllll\0",pop);
		   		*subN=params.head[tableNum];
		}
		p1=0;
		if (cmd[p1]==0 || cmd[p1]=='\n') // if the set means to delete
		{
			values[0][0]=0;
			return 0;
		}
		for (k=0;k<params.tableColCount[tableNum];k++){ // I think there is a better way to approach this
			j=0;
			p1=0;
			p2=0;
			while (cmd[p1]==' ')
			{p1++;} //while it is a space
			p2=p1;

			while (cmd[p2]!=0 &&cmd[p2]!='\n' && cmd[p2]!=' ')//find the end of the column
			{p2++;}
			if (cmd[p2]!=0 &&cmd[p2]!='\n'){
				if ((p2-p1+1)>=MAX_COLNAME_LEN)
				{
					printf ("column name is too long\n");
					return -2;
				}
				for (i=0;p1<p2;p1++)
				{
					columns[k][i]=cmd[p1];
					i++;
				}
			columns[k][i]=0;
			}
			//printf ("column %d is '%s' \n",k, columns[k]);
			cmd=cmd+p2;
			p1=0;
			while (cmd[p1]==' ')
				{p1++;}//while space
			p2=p1;
			if (cmd[p1]==0 ||cmd[p1]=='\n') // if it finds the end of line before expected
				{return -2;} // too few inputs
			else
				{p2++;}// help avoid the case of cmd[p1] == '-' or '+' b/c these values are not accepted for string
			while (cmd[p2]!=0 && cmd[p2]!='\n' && cmd[p2]!=',')//find the end of the value
			{
				if (cmd[p2]!=' '&&!((cmd[p2]>='0'&&cmd[p2]<='9')||(cmd[p2]>='A'&&cmd[p2]<='Z')||(cmd[p2]>='a'&&cmd[p2]<='z')))
					{return -2;	} // check for values not=0-9 A-Z a-z ' '
				p2++;
			}
			i=p2-1; //starts looking on the char before so
			int subp1=p1; // saves the value of p1 here
			while (cmd[i]==' ')
			{i--;}//backtrack to find the first not ' ' with i as sub for p2
			clen[k]=0; // sets the length to 0
			if (params.tableColLength[tableNum][k]!=0) //if it's not an int
			{
				if ((i-p1+2)>=params.tableColLength[tableNum][k])
				{printf ("values is too long \n"); // set length is too long
					return -2;}
			}
			else if ((i-p1+2)>=MAX_STRTYPE_SIZE)
			{
				printf ("values is too long \n"); // set length is too long
					return -2;

			}
			notStr=0;
			if (cmd[p1]=='-'|| cmd[p1]=='+') // int if true
				{ notStr=1;}
			values[k][0]=cmd[p1++]; // slight addition to compensate for starting j at 1
			for (j=1;p1<=i;p1++) // looks through the rest and stores it
			{
				values[k][j]=cmd[p1]; // stores the values
				if ((values[k][j]<'0' || values[k][j]>'9' )) // not an int
				{
					if (notStr)
						return -2;
					clen[k]=i+1-subp1; // change the column len b/c it's a string sets the string lengths
				}
				j++;
			}


			values[k][j]=0;
			printf ("value %d is '%s' \n",k, values[k]);
				cmd=cmd+p2;
			if ((*cmd==0 ||*cmd=='\n')&&k!=params.tableColCount[tableNum]-1)
				return -2;//too few columns
			if (*cmd!=0 &&*cmd!='\n'&&k==params.tableColCount[tableNum]-1)//if it hasn't reached end of line despite expectations.
				return -2;//too many columns

			cmd++; // in this case it's pointing to comma so skip it
		}
		if (compareInts(clen,params.tableColLength[tableNum],params.tableColCount[tableNum],1) !=0 ||compareStrings(columns,params.tableCol[tableNum],params.tableColCount[tableNum])!=0)
			{
				printf("the set values are not as expected\n"); // checks if length of strings are right and whether the columns match
				return -2;
			}
		if (k<MAX_COLUMNS_PER_TABLE){values[k][0]=0;}


return 0;//prob no error
}


/**
 * @brief Start the storage server.
 *
 * This is the main entry point for the storage server.  It reads the
 * configuration file, starts listening on a port, and proccesses
 * commands from clients.
 */
int main(int argc, char *argv[])
{
	if (global_logger==2)
	{
		time_t rawtime;
		struct tm * timeinfo;
		char title [80];

		time (&rawtime);
		timeinfo = localtime (&rawtime);

		strftime (title,80,"Server-%F-%H-%M-%S.log",timeinfo);
		puts (title);
		server_file = fopen(title,"w");
	}

	// Process command line arguments.
	// This program expects exactly one argument: the config file name.
	assert(argc > 0);
	if (argc != 2) {
		printf("Usage %s <config_file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char *config_file = argv[1];

	// Read the config file.

	int status = read_config(config_file, &params);
	if (status != 0) {
		printf("Error processing config file.\n");
		exit(EXIT_FAILURE);
	}
	//BST();
	{
		char string2[80];
		strcpy(string2, "Server On: Server Name: ");
		char string1[80];
		logger(server_file, strcat(string2, params.server_host));
		sprintf(string1, " Port Number is: %d\n",params.server_port);
		logger(server_file,string1);
	}
	// Create a socket.
	int listensock = socket(PF_INET, SOCK_STREAM, 0);
	if (listensock < 0) {
		printf("Error creating socket.\n");
		exit(EXIT_FAILURE);
	}

	// Allow listening port to be reused if defunct.
	int yes = 1;
	status = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	if (status != 0) {
		printf("Error configuring socket.\n");
		exit(EXIT_FAILURE);
	}

	// Bind it to the listening port.
	struct sockaddr_in listenaddr;
	memset(&listenaddr, 0, sizeof listenaddr);
	listenaddr.sin_family = AF_INET;
	listenaddr.sin_port = htons(params.server_port);
	inet_pton(AF_INET, params.server_host, &(listenaddr.sin_addr)); // bind to local IP address
	status = bind(listensock, (struct sockaddr*) &listenaddr, sizeof listenaddr);
	if (status != 0) {
		printf("Error binding socket.\n");
		exit(EXIT_FAILURE);
	}

	// Listen for connections.
	status = listen(listensock, MAX_LISTENQUEUELEN);
	if (status != 0) {
		printf("Error listening on socket.\n");
		exit(EXIT_FAILURE);
	}
	// Listen loop.
	int wait_for_connections = 1;
	while (wait_for_connections) {
		//sleep (1000);
		// Wait for a connection.
		struct sockaddr_in clientaddr;
		socklen_t clientaddrlen = sizeof clientaddr;
		int clientsock = accept(listensock, (struct sockaddr*)&clientaddr, &clientaddrlen);
		if (clientsock < 0) {
			printf("Error accepting a connection.\n");
			exit(EXIT_FAILURE);
		}

		{ //logger
			char string2[80];
			strcpy(string2, "Got a connection from: ");
			char string1[80];
			logger(server_file, strcat(string2, inet_ntoa(clientaddr.sin_addr)));
			sprintf(string1, " Client's Port Number is: %d\n",clientaddr.sin_port);
			logger(server_file,string1);
		}
		// Get commands from client.
		int wait_for_commands = 1;
		do {
			// Read a line from the client.
			char cmd[MAX_CMD_LEN];
			int status = recvline(clientsock, cmd, MAX_CMD_LEN);
			if (status != 0) {
				// Either an error occurred or the client closed the connection.

				{ //logger
					char string1[80];
					sprintf(string1, "Broken Pipe\n");
					logger(server_file,string1);
				}
				wait_for_commands = 0;
				break;
			} else {
				// Handle the command from the client.
				int status = handle_command(clientsock, cmd);
				if (status == 0)
					printf("Command Successful\n");
				else if (status == -1)
					printf("Login Failed\n");
				else if (status == -2)
					printf("Command Incorrect\n");
				else if (status == -3)
					printf ("Incorrect Information\n");
				else if (status == -5)
				{
					printf("client disconnected \n");
					wait_for_commands=0;
				}
				else if (status == -6)
				{
					printf("Send report failed\n");
					wait_for_commands=0;
				}
			}
		} while (wait_for_commands);
		Auth=0;
		/*{ // modifies the sorted file
			sorted_file = fopen("Sorted_file.txt","w");
       		printAll(params.head[2]);
        	fclose (sorted_file);
		}*/
		// Close the connection with the client.
		close(clientsock);

		{
			char string4[80];
			strcpy(string4, "Closed connection from: ");
			char string3[80];
			logger(server_file,strcat(string4, inet_ntoa(clientaddr.sin_addr)));
			sprintf(string3," Client's Port Number is: %d\n",clientaddr.sin_port);
			logger(server_file,string3);

		}
	}

	// Stop listening for connections.

	close(listensock);
	if(global_logger == 2)
	{
		fclose(server_file);
	}
	return EXIT_SUCCESS;
}

