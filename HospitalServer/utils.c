/**
 * @file
 * @brief The recvline works better that's it
 * can be used by the storage server and client library.
 */

#define _XOPEN_SOURCE


#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "utils.h"
int global_logger=0;
extern int errno;
int i [5] = {0,0,0,0,0};
int sendall(const int sock, const char *buf, const size_t len)
{
	size_t tosend = len;
	while (tosend > 0) {
		ssize_t bytes = send(sock, buf, tosend, 0);
		if (bytes <= 0)
			break; // send() was not successful, so stop.
		tosend -= (size_t) bytes;
		buf += bytes;
	}
	return tosend == 0 ? 0 : -1;
}

/**
 * In order to avoid reading more than a line from the stream,
 * this function only reads one byte at a time.  This is very
 * inefficient, and you are free to optimize it or implement your
 * own function.
 */
int recvline(const int sock, char *buf, const size_t buflen)
{
	int i;
	int status = 0; // Return status.
		ssize_t bytes = recv(sock, buf, buflen, 0);
		if (bytes <= 0) {
			// recv() was not successful, so stop.
			status = -1;
		}
		for ( i=0;(*buf != '\n')&&(*buf !='\0') && (i<buflen);i++){
			buf++;
			}
			// Found end of line, so stop.
			*buf = 0; // Replace end of line with a null terminator.
			status = 0;
	*buf = 0; // add null terminator in case it's not already there.
	return status;
}
int checkInput (char * table){
		int i=0;
		int foundLetter=0;
		if (table==NULL||table[0]==0)
			return -1;
		while (table[i]!='\n' && table[i]!=0 && i<sizeof(table))
		{
			if ((table[i]<='z'&&table[i]>='a')||(table[i]<='Z'&&table[i]>='A')||(table[i]<='9'&&table[i]>='0'))
				foundLetter=1;
			if (table[i++]==' ') // makes sure the key is one word
			{
				while (table[i]!='\n' && table[i]!=0 &&i<sizeof(table))
				{
					if ((table[i]<='z'&&table[i]>='a')||(table[i]<='Z'&&table[i]>='A')||(table[i]<='9'&&table[i]>='0'))
					{
						return -1;
					}
					i++;
				}
			}
		}
		if(!foundLetter)
		{
			return -1;
		}
return 0;
}

int sendReport (const int sock, int pass)
{
	int status = 0;
if (pass==0)
	 status = send(sock, "t", 1, 0);
else
	status = send(sock, "f", 1, 0);
 send(sock, "\n", 1, 0);
return status;
}

/**
 * @brief Parse and process a line in the config file.
 */
int process_config_line(char *line, struct config_params *params)
{
	if (line[0] == CONFIG_COMMENT_CHAR)
		return 0;
	// Extract config parameter name and table.
	char name[MAX_TABLE_LEN];
	char table[MAX_TABLE_LEN];
	int items = sscanf(line, "%s %s\n", name, table);

	// Line wasn't as expected.
	if (items != 2 && strcmp(name, "table") != 0){
		if (items<=0)
		return 0;
		else
		return -1;
	}
	// Process this line.
	if (strcmp(name, "server_host") == 0) {
		if (i[0] == 0)
		{
			i[0] = 1;
			strncpy(params->server_host, table, sizeof params->server_host);
		}
		else
		{
			printf ("server_host is already been input\n");
			return -1;
		}
	} else if (strcmp(name, "server_port") == 0) {
		if (i[1] == 0)
		{
			i[1] = 1;
			params->server_port = atoi(table);
		}
		else
		{
			printf ("server_port is already been input\n");
			return -1;
		}
	} else if (strcmp(name, "username") == 0) {
		if (i[2] == 0)
		{
			i[2] = 1;
			strncpy(params->username, table, sizeof params->username);
		}
		else
		{
			printf ("username is already been input\n");
			return -1;
		}

	} else if (strcmp(name, "password") == 0) {
		if (i[3] == 0)
		{
			i[3] = 1;
			strncpy(params->password, table, sizeof params->password);
		}
		else
		{
			printf ("password is already been input\n");
			return -1;
		}
	}
	else if (strcmp(name, "table") == 0) {
		int j=0;

		if (strlen(table) >= MAX_TABLE_LEN)
			return -1;
		for (j=0;j<i[4];j++)
		{
			if (strcmp(table, params->table[j]) == 0) { // check if TABLE NAME already exists
				struct config_params params2;
				strncpy(params2.table[i[4]], table, sizeof params2.table[i[4]]);
				params2.tableCount=0;
					if (0!=marshallData (&params2,line))
						return -1;
				if (compareInts(params->tableColLength[j],params2.tableColLength[0],params->tableColCount[j],0) !=0 || compareStrings(params->tableCol[j],params2.tableCol[0],params->tableColCount[j])!=0)
					return -1;  // if it is compare the columns
				else
					return 0;
			}
		}
		strncpy(params->table[i[4]], table, sizeof params->table[i[4]]);
		params->head[i[4]]=NULL;
		params->tableCount=i[4];
		i[4]++;
		if (i[4]>MAX_TABLES)
			return -1;
		return marshallData (params,line);
	}
	else {
		printf("Error: Invalid parameter\n");
		return -1;
	}
	memset(name,0,strlen(name));
	memset(table,0,strlen(table));
	return 0;
}

int read_config(const char *config_file, struct config_params *params)
{
	int error_occurred = 0;
	params->tableCount=0;
	// Open file for reading.
	FILE *file = fopen(config_file, "r");
	if (file == NULL)
		error_occurred = 1;
	// Process the config file.
	while (!error_occurred && !feof(file)) {
		// Read a line from the file.
		char line[MAX_CONFIG_LINE_LEN];
		char *l = fgets(line, sizeof line, file);
		// Process the line.
		printf ("%s",line);
		if (l == line)
		{
				error_occurred = process_config_line(line, params);
				/*if (error_occurred<0)
					printf ("error occured \n");
				error_occurred=0;*/
		}
		else if (!feof(file))
			error_occurred = -1;
	}
	if (error_occurred==0)
	{
	int j=0;
	for (j=0;j<5;j++)
		if (i[j]<=0)
		{
			printf ("Not enough valid inputs\n");
			return -1;
		}
	}
	int i=0;
	int j=0;
	for (i=0;i<params->tableCount;i++)
	{
		printf ("table%d: '%s' \n",i,params->table[i]);
		for (j=0;j<params->tableColCount[i];j++)
		{
			printf ("column%d: '%s' length: '%d' ||",j,params->tableCol[i][j],params->tableColLength[i][j]);
		}
		printf (" \n");
	}
	return error_occurred;
}

int marshallData (struct config_params *params,char *cmd) // what about a char with length 0??
{
	int i=0;
	int j=0;
	int check=0;
	char name[MAX_TABLE_LEN];
		params->tableColCount[params->tableCount]=0;
		for (j=0;j<2;j++)
		{
			while ((cmd[i]!=' '||cmd[i+1]==' '))
			{
				i++;
			}
			i++;
		}
		while (1){
		j=0;
		while ((cmd[i]<='z'&&cmd[i]>='a')||(cmd[i]<='Z'&&cmd[i]>='A')||(cmd[i]<='9'&&cmd[i]>='0'))
		{ //records the name of the column
			check=1;
			params->tableCol[params->tableCount][params->tableColCount[params->tableCount]][j]=cmd[i];
			i++;
			j++;
			if (j>= MAX_COLNAME_LEN) // if there are too many characters in column name
			{
				return -1;
			}
		}
		i++;
		params->tableCol[params->tableCount][params->tableColCount[params->tableCount]][j]=0;
		//printf("table: %s column: %s \n",params->table[params->tableCount],params->tableCol[params->tableCount][params->tableColCount[params->tableCount]]);
		if (check)
		{ //makes sure the column has a name
			params->tableCol[params->tableCount][params->tableColCount[params->tableCount]][j]=0;
			params->tableColLength[params->tableCount][params->tableColCount[params->tableCount]]=0;
			if (cmd[i]=='i'&&cmd[i+1]=='n'&&cmd[i+2]=='t') i+=3; // if it is an int do nothing
			else if (cmd[i]=='c'&&cmd[i+1]=='h'&&cmd[i+2]=='a'&&cmd[i+3]=='r'&&cmd[i+4]=='[') // if it is a char record the char length
			{
				i+=5;  // there should be no problem with this shift
				while (cmd[i]<='9' &&cmd[i]>='0') // sets the char length
				{
					params->tableColLength[params->tableCount][params->tableColCount[params->tableCount]]*=10;
					params->tableColLength[params->tableCount][params->tableColCount[params->tableCount]]+=cmd[i]-48;
					i++;
				}

				if (params->tableColLength[params->tableCount][params->tableColCount[params->tableCount]]>MAX_STRTYPE_SIZE)
					return -1;
				else if (params->tableColLength[params->tableCount][params->tableColCount[params->tableCount]]<=0)
					return -1;
				i++;
			}
			else // if neither error
			{
				return -2;
			}
			for (j=0;j<params->tableColCount[params->tableCount];j++) // check for duplicate columns
			{//makes sure it goes from 0 to the one before it to look for duplicate column names
				if (strcmp(params->tableCol[params->tableCount][j],params->tableCol[params->tableCount][params->tableColCount[params->tableCount]])==0)
					return -1;
			}
			if (params->tableColCount[params->tableCount]>=MAX_COLUMNS_PER_TABLE) //check iftoo many tables
					return -1;
		//	printf("string length %d \n",params->tableColLength[params->tableCount][params->tableColCount[params->tableCount]]);
			params->tableColCount[params->tableCount]++; //number of columns +1

			while (cmd[i]!=',') //look for comma or the end
			{
				if (cmd[i]=='\0' ||cmd[i]=='\n') // check to see if string ends
				{
					params->tableCount++;
					return 0;
				}
				else if (cmd[i]!=' ')
				{
					return -1;
				}
				i++;
			}
			while (!((cmd[i]<='z'&&cmd[i]>='a')||(cmd[i]<='Z'&&cmd[i]>='A')||(cmd[i]<='9'&&cmd[i]>='0')))
			{
				if (cmd[i]=='\0' ||cmd[i]=='\n') // check to see if string ends
					return -6;
				i++;
			}
			check=0;
		}
		else
		{
			return -3;
		}
		if (params->tableColCount[params->tableCount]==0) // if there are no col count then check failed
			return -4;
		}
	return-5; // default is to assume error
}

int compareInts(const int * clen,const int * clen2,int count,int setting)
{
	int i=0;
	if (setting)
	{
		for (i=0;i<count;i++)
		{
			//printf("int1:%d int2:%d \n", clen[i],clen2[i]);
			if (clen[i]>clen2[i])
				return -1;
		}
	}
	else
	{
		for (i=0;i<count;i++)
		{
			//printf("int1:%d int2:%d \n", clen[i],clen2[i]);
			if (clen2[i]!=0 && clen[i]!=clen2[i])
				return -1;
		}
	}
	return 0;
}

int compareStrings(char clen [MAX_COLUMNS_PER_TABLE] [MAX_COLNAME_LEN],char clen2 [MAX_COLUMNS_PER_TABLE][MAX_COLNAME_LEN],int count)
{
	int i=0;

	for (i=0;i<count;i++)
	{
		//printf("s1: %s s2:%s \n", clen[i],clen2[i]);
		if (strcmp (clen[i],clen2[i])!=0)
			return -1;
	}
	return 0;
}

void logger(FILE *file, char *message)
{
	if(global_logger == 1)
	{
		printf("%s", message); // replace LOG commands with logger() calls
	}
	if(global_logger == 2)
	{
	fprintf(file,"%s",message);
	fflush(file);
	}
}

char *generate_encrypted_password(const char *passwd, const char *salt)
{
	if(salt != NULL)
		return crypt(passwd, salt);
	else
		return crypt(passwd, DEFAULT_CRYPT_SALT);
}
