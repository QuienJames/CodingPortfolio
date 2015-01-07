/**
 * @file
 * @brief This file contains the working implementation of the storage server
 * interface as specified in storage.h.
 * Everything works and checks errors and communicates with the server nicely
 */
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "storage.h"
#include "utils.h"
extern int global_logger;
extern int errno;
extern FILE* client_file;

/**
 * @Tries to connect to server
 */

void* storage_connect(const char *hostname, const int port)
{
	int i=0;
	// Create a socket.
	errno=0;
	if (hostname == NULL||port<=0)
	{
		errno=ERR_INVALID_PARAM;
		return NULL;
	}
	while(hostname[i]!=0 && i< strlen(hostname))
	{
		i++;
	}
	if (hostname[i] != 0)
	{
		errno=ERR_INVALID_PARAM;
		return NULL;
	}
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock <0) //Just in case this can happen, because it should not
	{
		errno=ERR_CONNECTION_FAIL;
		return NULL;
	}
	// Get info about the server.
	struct addrinfo serveraddr, *res;
	memset(&serveraddr, 0, sizeof serveraddr);

	/*if(global_logger == 2)
	{
		char string2[80];		// Placed logger here it here because had to make sure that memory set was sucessfull
		strcpy(string2, "Connected to Server: Server Name: ");     
		char string1[80];
		logger(client_file, strcat(string2, hostname));
		sprintf(string1, " Port Number is: %d",port);
		logger(client_file,string1);
	}*/
	
	
	serveraddr.ai_family = AF_UNSPEC;
	serveraddr.ai_socktype = SOCK_STREAM;
	char portstr[MAX_PORT_LEN];
	snprintf(portstr, sizeof portstr, "%d", port);
	int status = getaddrinfo(hostname, portstr, &serveraddr, &res);
	if (status != 0)
	{
		errno=ERR_UNKNOWN;
		return NULL;
	}
	// Connect to the server.
	status = connect(sock, res->ai_addr, res->ai_addrlen);
	if (status != 0){
		errno = ERR_CONNECTION_FAIL;
		return NULL;
	}
	return (void*) sock;
}

/**
 * @Tries to login
 */

int storage_auth(const char *username, const char *passwd, void *conn)
{
	errno=0;
	// Connection is really just a socket file descriptor.
	if(conn==NULL) //if it is not connected to the server
		{
			errno = ERR_INVALID_PARAM;
			return -1;
		}
	int sock = (int)conn;
	if(sock<0)
		{
			errno = ERR_INVALID_PARAM;
			return -1;
		}
	// Send some data.
	char buf[MAX_CMD_LEN];
	memset(buf, 0, sizeof buf);
	char *encrypted_passwd = generate_encrypted_password(passwd, NULL);
	snprintf(buf, sizeof buf, "AUTH %s %s \n", username, encrypted_passwd);
	if (sendall(sock, buf, strlen(buf)) == 0 && recvline(sock, buf, sizeof buf) == 0) {
		if (buf[0]=='A'&&buf[1]=='U'&&buf[2]=='T')
		{
			errno=ERR_CONNECTION_FAIL; //no responce from server so connection failed
			return -1;
		}
		if (buf[0] == '-') // if the first char is negative it fails
		{
			errno = ERR_AUTHENTICATION_FAILED;
			return -1;
		}

		return 0;
	}
	errno=ERR_CONNECTION_FAIL;
	return -1;
}

/**
 * @Tries to get something froma table
 */

int storage_get(const char *table, const char *key, struct storage_record *record, void *conn)
{
	errno=0;
	// Connection is really just a socket file descriptor.
	if(conn==NULL||table==NULL||key==NULL)
		{
			errno = ERR_INVALID_PARAM;
			return -1;
		}
	int sock = (int)conn;
	if(sock<0)
		{
			errno = ERR_INVALID_PARAM;
			return -1;
		}


if (checkInput (table)!=0)
{
	errno= ERR_INVALID_PARAM;;
	return -1;
}

if (checkInput (key)!=0 || strlen(key) >= MAX_KEY_LEN)
{
	errno= ERR_INVALID_PARAM;;
	return -1;
}
	// Send some data.
	char buf[MAX_CMD_LEN];
	memset(buf, 0, sizeof buf);
	/* if(global_logger == 2) //Checking if user function memory set was sucessfull
	{
		char string6[80];
		strcpy(string6,"\nGet data sucessfully from table: ");
		logger(client_file,strcat(string6,table)); // if getting data was sucessfull	
	}	
	 */
	snprintf(buf, sizeof buf, "GET %s&%s \n", table, key);
	//printf ("%s\n",buf);
	if (sendall(sock, buf, strlen(buf)) == 0 && recvline(sock, buf, sizeof buf) == 0) {
		if (buf[0]=='G'&&buf[1]=='E'&&buf[2]=='T')
		{
			errno=ERR_CONNECTION_FAIL; //no responce from server so connection failed
			return -1;
		}
		if (buf[0] == '-'){
			if (buf[1]=='3') //the negative number returned are different ways to organize error
				errno = ERR_TABLE_NOT_FOUND;
			else if (buf[1]=='4')
				errno = ERR_KEY_NOT_FOUND;
			else if (buf[1]=='7')
				errno = ERR_NOT_AUTHENTICATED;
			//printf ("buf: %s\n",buf);
			return -1;
		}
		strncpy(record->value, buf, sizeof record->value);
		//printf ("value: %s\n",record->value);
		return 0;
	}
	else
	errno=ERR_CONNECTION_FAIL; //no responce from server so connection failed
	return -1;
}


/**
 * @
 */

int storage_set(const char *table, const char *key, struct storage_record *record, void *conn)
{
	errno=0;
	// Connection is really just a socket file descriptor.
	if(conn==NULL||table==NULL||key==NULL) // If not connected
		{
			errno = ERR_INVALID_PARAM;
			return -1;
		}
	int sock = (int)conn;
	if(sock<0)
		{
			errno = ERR_INVALID_PARAM;
			return -1;
		}
if (checkInput (table)!=0)
{
	errno= ERR_INVALID_PARAM;;
	return -1;
}


if (checkInput (key)!=0 || strlen(key) >= MAX_KEY_LEN)
{
	errno= ERR_INVALID_PARAM;;
	return -1;
}

	// Send some data.
	char buf[MAX_CMD_LEN];
	memset(buf, 0, sizeof buf);
	/*
	if(global_logger == 2)		//Logger here to check if user function memory set was sucessfull
	{
		char string6[80];
		strcpy(string6, "\nSet Sucessful from table: ");
		logger(client_file,strcat(string6, table));
	}*/
	if (record==NULL)
	{
		snprintf(buf, sizeof buf, "SET %s&%s&\n", table, key);
	}
	else
	{
		snprintf(buf, sizeof buf, "SET %s&%s&%s\n", table, key,record->value);
	}
	//printf ("%s",buf);
	if (sendall(sock, buf, strlen(buf)) == 0 && recvline(sock, buf, sizeof buf) == 0) {
		if (buf[0]=='S'&&buf[1]=='E'&&buf[2]=='T')
		{
			errno=ERR_CONNECTION_FAIL; //no responce from server so connection failed
			return -1;
		}
		if (buf[0] == '-'){
			if (buf[1]=='3') //error codes
				errno = ERR_TABLE_NOT_FOUND;
			else if (buf[1]=='4')
				errno = ERR_KEY_NOT_FOUND;
			else if (buf[1]=='2') //error codes
				errno = ERR_INVALID_PARAM;
			else if (buf[1]=='7')
				errno = ERR_NOT_AUTHENTICATED;
			//printf ("set is %s \n",buf);
			return -1;
		}
		//printf ("set is %s \n",buf);
		return 0;
	}
	else
	errno=ERR_CONNECTION_FAIL;
	return -1;
}
/*
int storage_load(const char *table, const char *file, void *conn)
{
	// Connection is really just a socket file descriptor.
	int sock = (int)conn;
	// Send some data.
	char buf[MAX_CMD_LEN];
	memset(buf, 0, sizeof buf);
	snprintf(buf, sizeof buf, "LOAD %s&%s \n", table, file);
	if (sendall(sock, buf, strlen(buf)) == 0 && recvline(sock, buf, sizeof buf) == 0) {
		if (buf[0] == 'f')
		return -1;
		return 0;
	}
	return -1;
}*/
/**
 * @brief Breakup with the server and checks for a responce
 */

int storage_disconnect(void *conn)
{
	errno=0;
	// Cleanup
	if(conn==NULL)
		{
			errno = ERR_INVALID_PARAM;
			return -1;
		}
	int sock = (int)conn;
	if(sock<0)
		{
			errno = ERR_INVALID_PARAM;
			return -1;
		}
	char buf[MAX_CMD_LEN];
	snprintf(buf, sizeof buf, "DISCONNECT \n");
	if (sendall(sock, buf, strlen(buf)) == 0 && recvline(sock, buf, sizeof buf) == 0) {
		close(sock);
		if (buf[0]!='-') // disconnect now sends a message to the server so both are on the same page
			return -1;
	}
	else
	{
		errno=ERR_CONNECTION_FAIL;
		return -1;
	}
	conn=0;
	/*
	if(global_logger == 1)
	{
		printf("Disconnected from server...\n");
		return 0;
	}
	if(global_logger == 2)		//Optional Logger
	{
		logger(client_file, "\nDisconnecting from server..."); // if sucessfully disconnected, will print this message in logger
		return 0;
	}	
	*/
	return 0;
}

int storage_query(const char *table, const char *predicates, char **keys,const int max_keys, void *conn)
{

/* ok you can approach this by looking at how the gV function in server.c works and get in node.c
 * essentially  you just have to use the printall algorithm to search through and send back the right values
 *
*/

	int numberKeys=-1;	
	int sock = (int)conn;	
	char buf[1000];
   	int i;
	//keys = (char**) malloc(max_keys*sizeof(char*));
   	for (i = 0; i < max_keys; i++)
   	{
    	   keys[i] = (char*) malloc(MAX_KEY_LEN*sizeof(char));
   	}
	snprintf(buf, sizeof buf, "Query %s&%d&%s",table,max_keys, predicates);
	strcat (buf,"\n");
	if ((sendall(sock, buf, strlen(buf)) == 0)&&(recvline(sock, buf, sizeof buf) == 0))
	{
		i = 0;
		if (buf[0] == '-')
		{
			if (buf[1]=='3') //error codes
				errno = ERR_TABLE_NOT_FOUND;
			else if (buf[1]=='4')
				errno = ERR_KEY_NOT_FOUND;
			else if (buf[1]=='2') //error codes
				errno = ERR_INVALID_PARAM;
			else if (buf[1]=='7')
				errno = ERR_NOT_AUTHENTICATED;
			return -1;
		}
		else
		{
			sscanf(buf,"%d", &numberKeys);
			//printf ("key count : %d\n",numberKeys);
		}
		i=0;
		while(i<max_keys)
		{
			snprintf(buf, sizeof buf,"OK\n");
			if((sendall(sock, buf, strlen(buf)) == 0)&&(recvline(sock, buf, sizeof buf) == 0))
			{
				//printf ("buf %d:'%s'\n",i,buf);
			    strcpy( keys[i],buf);
			    //printf ("key %d:'%s'\n",i,keys[i]);
				if(*buf == 0)
					{
					return numberKeys;}
				i++;
			}
		}
		return numberKeys;	
	}
	return -1;
}
