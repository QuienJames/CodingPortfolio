/**
 * @file
 * @brief This file implements a "very" simple sample client.
 * 
 * The client connects to the server, running at SERVERHOST:SERVERPORT
 * and performs a number of storage_* operations. If there are errors,
 * the client reports them and gives you another chance
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#define SERVERHOST "localhost"
#define SERVERPORT 1113
#define SERVERUSERNAME "admin"
#define SERVERPASSWORD "dog4sale"
#define TABLE "marks"
#define KEY "ece297"
#define LOGGER_2 0
FILE* client_file;
extern int global_logger;
extern int errno;

bool isInteger2(const char *str)
{
	if(*str == '-')
		{return false;}
		
	if(str == NULL)
		{return false;}
	while(*str)
	{
		if(!isdigit(*str))
		{
			return false;		
		}
		str++;	
	}
return true;
}

int main(int argc, char *argv[]) {
global_logger = LOGGER_2;
if(global_logger == 2)
{
	time_t rawtime;
	struct tm * timeinfo;
	char title1 [80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (title1,80,"Client-%F-%H-%M-%S.log",timeinfo);
	puts (title1);
	client_file = fopen(title1,"w");	
}

void* conn=NULL;
int status;
int authentication = 0;
int testing=0;
char max_keys[5];
char table[MAX_TABLE_LEN];
char key[MAX_KEY_LEN];
char new_value[MAX_VALUE_LEN];
char user_choice[5];
char* keys[20];
struct storage_record r;
  printf("Welcome to the Server\n");
  printf("--------------------------------\n");
  printf("1) Connect\n2) Authenticate\n3) Get\n4) Set\n5) Disconnect\n6) Exit\n7) Query\n");
  printf("--------------------------------\n");

  //scanf ("%s",testCase);
  FILE *file;// open testcase
  while(*user_choice != '6')
  	{
	  	 if (testing!=1)
	  	 {
		printf("Please enter your selection: ");
		scanf("%s", user_choice);
	  	 }
		if (*user_choice=='8' ||testing)
		{
			if (testing==0)
			{
				file = fopen("tester.txt", "r");
			}
			testing=1;
			int error_occurred = 0;
			char line[MAX_CONFIG_LINE_LEN];
			char *l = fgets(line, sizeof line, file);
				if (file == NULL)
					error_occurred = 1;
				// Process the config file.
				if (error_occurred || feof(file)) {
					testing=0;
					fclose(file);
				}
				else
				{
					sscanf(line, "%s %s %s\n",user_choice,table,key);
					printf("%s %s %s\n",user_choice,table,key);
				}
			memset(line,0,strlen(line));
			if (*user_choice == '4')
			{
				l = fgets(new_value, sizeof new_value, file);
				printf("%s",new_value);
			}
			else if (*user_choice == '7')
			{
				l = fgets(new_value, sizeof new_value, file);
				printf("%s",new_value);
				l = fgets(line, sizeof line, file);
				sscanf(line, "%s \n",max_keys);
				printf("%s\n",max_keys);
			}
			if (error_occurred==-1)
			{
				printf ("error occured \n");
			}
		}
		if (*user_choice == '1')
			{

			    	// Connect to server
				char host_name[MAX_HOST_LEN];
				char port_number[MAX_PORT_LEN];
				//printf("Please input the hostname: ");
				//scanf("%s",host_name);
				//printf("Please input the port: ");
				//scanf("%s",port_number);
				strcpy (host_name, table);
				strcpy (port_number,key);
				conn = storage_connect(host_name,atoi(port_number));
  				if(!conn) {
					errno = ERR_CONNECTION_FAIL;
    					printf("Cannot connect to server @ %s:%d. Error code: %d.\n",host_name,atoi(port_number), errno);
					}
			}
		else if(conn == NULL)
		{
			if(*user_choice == '6')
			{
				return 0;
			}
			errno = ERR_INVALID_PARAM;
			printf("NULL CONNECTION PARAMETER\n"); // Not sure if this is correct
		}
		else if(*user_choice == '5' ||*user_choice == '6')
		{
				// Disconnect from server
				if(conn == NULL)
				{
					errno = ERR_INVALID_PARAM;
					printf("NULL CONNECTION PARAMETER\n"); // Not sure if this is correct
				}
				else {
				status = storage_disconnect(conn);
  				if(status != 0) {
    				printf("storage_disconnect failed. Error code: %d.\n", errno);
    				return status;
  				}
  				else {
  					if(global_logger == 1)
  					{
  						printf("storage_disconnect: successful.\n");
  					}
  				conn=0; // added disconnect
  				authentication=0;
  				}
  				if(*user_choice == '6')
  				{return 0;}
				}
		}
		else if(*user_choice == '2')
		{
				// Authenticate the client.
  				char user_name[MAX_USERNAME_LEN];
				char password[MAX_ENC_PASSWORD_LEN];
				//printf("Please input username: ");
				//scanf("%s", user_name);
				//printf("Please input password: ");
				//scanf("%s",password);
				strcpy (user_name, table);
				strcpy (password, key);
				status = storage_auth(user_name, password, conn);
				if(status != 0) {
    					printf("storage_auth failed with username '%s' and password '%s'. " \
           				"Error code: %d.\n",user_name, password, errno);
    					//storage_disconnect(conn);
   				 	//return status;
  					}
				else {
				if(global_logger == 1)
				{
					printf("storage_auth: successful.\n");
				}
				authentication = 1; // Sucessfully authenticated the client
				}
		}
		else if (authentication!=1)
		{
			printf("storage_auth: Not Verified, Please login first.\n");
			errno = ERR_NOT_AUTHENTICATED; 
		}
		else if(*user_choice =='7')
		{
			if (testing==0){
				printf("Please input the table name: ");
				scanf("%s",table);
				printf("Please input the predictes: "); // there may be more than 1 condition
				scanf("%s",new_value);
				printf("Please input the number of keys: ");
				scanf("%s",max_keys);
			}
			printf ("number of keys is '%d' \n", storage_query(table, new_value, keys,atoi(max_keys),conn));
		}
		else if(*user_choice == '3')
		{
			// Issue storage_get
			if (testing==0){
			printf("Please input the table name: ");
			scanf("%s",table);
			printf("Please input the key: ");
			scanf("%s",key);
			}
			status = storage_get(table, key, &r, conn);
			if(conn == NULL)
			{
				errno = ERR_INVALID_PARAM;
				printf("NULL CONNECTION PARAMETER\n"); // Not sure if this is correct
			}

			if(status != 0) {
    			printf("storage_get failed. Error code: %d.\n", errno);
  			}
			else
			{
				printf("storage_get: the value returned for key '%s' is '%s'.\n",key, r.value);
			}
		
		}
		else if(*user_choice =='4')
			{
				// Issue storage_set
				if (testing==0){
				printf("Please input the table name: ");
				scanf("%s",table);
				printf("Please input the key: ");
				scanf("%s",key);
				printf("Please enter the value you would like to rewrite: ");
				scanf("%s",new_value);
				}
  				strncpy(r.value, new_value, sizeof r.value);
  				status = storage_set(table, key, &r, conn);
				if(conn == NULL)
				{
					errno = ERR_INVALID_PARAM;
					printf("NULL CONNECTION PARAMETER\n"); // Not sure if this is correct
				}
				if(status != 0) 
				{
    					printf("storage_set failed. Error code: %d.\n", errno);
  				}
				else
				{
					printf("storage_set: successful.\n");
				}
			}

		else
			{
				printf("Error: Invalid Choice\n");
			}
		*user_choice=0;
		memset(new_value,0,strlen(new_value));
		memset(table,0,strlen(table));
		memset(key,0,strlen(key));
	}
   
if(global_logger == 2)
{
	fclose(client_file);
}
  // Exit
  return 0;
}
