/**
 * @file
 * @Basic Binary Search Tree code
*/

#include "BST.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <string.h>
extern FILE*sorted_file; 

node* createNode(char*name,char population [MAX_COLUMNS_PER_TABLE] [MAX_STRTYPE_SIZE])
{
    int i=0;
    int j=0;
    node* newNode = (node*)malloc(sizeof(node));
    while (name[i]!=0)
    {
   		newNode->name[i]= name[i];
   		i++;
    }
    newNode->name[i]=0;

    i = 0;

    while (population[i][0]!=0)
    {
    	j=0;
    	while (population [i][j]!=0)
    	{
    		newNode->population[i][j]= population[i][j];
    		j++;
    	}
    	newNode->population[i][j]=0;
    	i++;
    }
    if (i<MAX_COLUMNS_PER_TABLE) newNode->population[i][0]=0;

    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
    
}


void BSTQuery(node* head, char parameters[MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE+4], const int max_keys, int* numKeys, char keys[max_keys][MAX_KEY_LEN], const int numParams)
{
    if(head == NULL)
	    {
	        return;
	    }
        
	if(head->left!=NULL)
        {
           BSTQuery(head->left, parameters, max_keys, numKeys, keys,numParams);
        }
        //printf("left");
        
        if(head->right!=NULL)
        {
            BSTQuery(head->right, parameters, max_keys, numKeys, keys,numParams);
        }
        //printf("right");
        
    if(strcmp(head->name,"Llllllllllllll\0")!=0)
    {
	    if (checkParameters(head, parameters,numParams))
		{	
			if(*numKeys<max_keys)
			{
				snprintf(keys[*numKeys], MAX_KEY_LEN,"%s",head->name);
			}
			*numKeys = *numKeys + 1;
		}
	}
	return;
}

bool checkParameters(node* head, char parameters[MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE+4], const int numParams)
{
	printf("checkParameters\n");
	bool check = true;
	
	int i=0;
	int numColumn=0;
	printf("%d %c\n", numParams, parameters[i][0]);
	for(i=0;i<numParams && parameters[i][0]!=0;i++)
	{
		printf("For Loop\n");
		numColumn = parameters[i][0] - '0';
		
		int k = 4;
		int l = 0;
		char parameterToCompare[MAX_STRTYPE_SIZE];
		while(parameters[i][k]!=0 && l<MAX_STRTYPE_SIZE)
		{
			parameterToCompare[l] = parameters[i][k];
			l++;
			k++;
		}
		parameterToCompare[l] = 0;
		l--;
		while(parameterToCompare[l] == ' ')
		{
		parameterToCompare[l] = 0;
		l--;
		}
		printf("parameterToCompare: %s population: %s \n", parameterToCompare, head->population[numColumn]);
		
		if((parameterToCompare[0]<='9'&&parameterToCompare[0]>='0')||parameterToCompare[0]=='-'||parameterToCompare[0]=='+')
		{
			int intPar = atoi (parameterToCompare);
			int intPop = atoi (head->population[numColumn]);
			
			printf("intParameter: %d, intPopulation: %d \n", intPar, intPop);
			
			if (parameters[i][2]=='=' && intPop!=intPar)
			{
				check = false;
				printf("false\n");
				return check;
			}
			else if (parameters[i][2]=='<' && intPop>=intPar)
			{
				check = false;
				printf("false\n");
				return check;
			}
			else if (parameters[i][2]=='>' && intPop<=intPar)
			{
				check = false;
				printf("false\n");
				return check;
			}
		}
		else
		{
			printf("Parameter to Compare: %s, Node Value: %s", parameterToCompare, head->population);
			
			if (parameters[i][2]=='=' && strcmp(head->population[numColumn],parameterToCompare)!=0)
			{
				check = false;
				printf("false\n");
				return check;
			}
			else if (parameters[i][2]=='<' && strcmp(head->population[numColumn],parameterToCompare)<=0)
			{
				check = false;
				printf("false\n");
				return check;
			}
			else if (parameters[i][2]=='>' && strcmp(head->population[numColumn],parameterToCompare)>=0)
			{
				check = false;
				printf("false\n");
				return check;
			}
		}
		
	}
	
	return check;
}

node* deleteAll(node* head)
{
	if (head==NULL)
		return NULL;
		
	if(head->left!=NULL)
		head->left = deleteAll(head->left);
		
	if(head->right!=NULL)
		head->right = deleteAll(head->right);
		
	if ((head->left==NULL)&&(head->right==NULL))
	{
		free(head);
		return NULL;
	}
	return NULL;
}

void printAll(node* head)
{
	/*if ((head->left==NULL)&&(head->right==NULL))
	{
		printf("%s %d \n", head->name,head->population);
                return;
	}*/

        if(head == NULL)
        {
            return;
        }
        //fprintf(sorted_file,"%s %s\n",head->name,head->population[0]);
        printf("name: %s pop[0]:%s\n",head->name,head->population[0]);
	if(head->left!=NULL)
        {
            printAll(head->left);
        }
        

        if(head->right!=NULL)
        {
            printAll(head->right);
        }
        
	return;
}
	
node* insert(node* head, char * name,char population [MAX_COLUMNS_PER_TABLE] [MAX_STRTYPE_SIZE])
{
	node* current = head;
        node* previous = NULL;
        int check_less;
	if (head == NULL)
	{
            return createNode (name,population);
	}
	
        while(current != NULL)
        {
            previous = current;
            check_less=strcmp(current->name, name);
            if(check_less>0)
            {
                current = current->left;                
            }
            else if(check_less<0)
            {
                current = current ->right;
            }  
            else 
               {
                printf ("Already exists\n");
                return head;
                }
               
        }
        
        if(check_less>0)
        {
            previous->left = createNode (name,population);
        }
        else if (check_less<0)
        {
            previous->right = createNode (name,population);
        }
        return head;
        
}

node* get(node* head, char* name,char* value[])
{
	if(head == NULL)
	{
		if (value!=NULL) printf("Error: Not Found\n");
		return NULL;                            // Return NULL if not found
	}
	else if(strcmp(name,head->name) < 0)
	{
		return get(head->left, name,value);
	}
	else if(strcmp(name,head->name) > 0)
	{
		return get(head->right, name,value);
	}
	else
	{
		if (value!=NULL)
		{
		int i=0;
		for(i=0;i<MAX_COLUMNS_PER_TABLE && head->population[i][0]!=0;i++)
		{
				value[i]=head->population[i];
		}
		}
	}
        return head;
}

node* set(node*head, char*name, char population [MAX_COLUMNS_PER_TABLE] [MAX_STRTYPE_SIZE])
{
        if(population == NULL || population[0][0]==0)
        {
            return deleteNode(head,name);
        }
        node* temp = get(head,name,NULL);
	if(temp == NULL)
	{
		return insert(head, name, population);
	}
	else 
	{
		int i = 0;
		int j=0;
		for (i=0;i<MAX_COLUMNS_PER_TABLE && population[i][0]!=0;i++)
		{
			for(j=0;population[i][j]!=0;j++)
			{
				temp->population[i][j]= population[i][j];
			}
	    temp->population[i][j]=0;
		}
		if (i<MAX_COLUMNS_PER_TABLE) temp->population[i][0]=0;
		return head;
	}
}


node* findPrevious(node* previous, char* name_)
{
	if(previous == NULL)
	{
		printf("Error: Not Found\n");
		return NULL;
	}
	else if(strcmp(name_,previous->name) < 0)
	{
		if(strcmp(name_,previous->left->name)==0)
			return previous;
		else
			return findPrevious(previous->left, name_);
	}
	else if(strcmp(name_,previous->name) > 0)
	{
		if(strcmp(name_,previous->right->name)==0)
			return previous;
		else
			return findPrevious(previous->right, name_);	
	}
	else
	{
		printf("No Previous\n");		
		return NULL;
	}	
}

node* deleteNode(node*head, char*name_)
{
	node* temp = get(head,name_,NULL);
	node* previous;
	if(temp == NULL)
	{
		printf("Error: Sorry key does not exist\n");
		return NULL;                    // Returns NULL if not found
	}
	else if(temp == head)
	{

		if(temp -> left == NULL && temp ->right == NULL)
		{			
			free(head);
			return temp;
		}
		else if(temp -> left!= NULL && temp ->right ==NULL)
		{
			temp = temp -> left;
			free(head);
			return temp;
		}
		else if(temp ->left == NULL && temp->right!=NULL)
		{
			temp = temp -> right;
			free(head);
			return temp;
		}
		else
		{
			node* smallest = findSmallest(temp->right);
			node* smallestPrev = findPrevious(head, smallest->name);
		//	smallestPrev->left = NULL;
			
			if(temp->right==smallest)
			{
				smallest->left = temp->left;
				temp = smallest;
				free(head);
				return temp;
			}
			else
			{
				smallestPrev->left=smallest->right;
				smallest->left=temp->left;
				smallest->right = temp->right;
				temp = smallest;
				free(head);
				return temp;
			}	
		}
	}

	previous = findPrevious(head, name_);
	

	if((previous->left!=NULL)&&(strcmp(previous->left->name,name_)==0))
	{
		if(temp -> left == NULL && temp ->right == NULL)
		{
			previous ->left = NULL;			
			free(temp);
			return head;
		}
		else if(temp -> left!= NULL && temp ->right ==NULL)
		{
			previous ->left = temp->left;
			free(temp);
			return head;
		}
		else if(temp ->left == NULL && temp->right!=NULL)
		{
			previous ->left = temp->right;
			free(temp);
			return head;
		}
		else
		{
			node* smallest = findSmallest(temp->right);
			node* smallestPrev = findPrevious(head, smallest->name);
		//	smallestPrev->left = NULL;
			if(temp->right==smallest)
			{
				smallest->left = temp->left;
				previous->left = smallest;
				free(temp);
				return head;
			}
			else
			{
				smallestPrev->left=smallest->right;
				smallest->left=temp->left;
				smallest->right = temp->right;
				previous->left = smallest;
				free(temp);
				return head;
			}	

		}
	}
	else
	{
		if(temp -> left == NULL && temp ->right == NULL)
		{
			previous->right = NULL;			
			free(temp);
			return head;
		}
		else if(temp -> left!= NULL && temp ->right ==NULL)
		{
			previous->right = temp->left;
			free(temp);
			return head;
		}
		else if(temp ->left == NULL && temp->right!=NULL)
		{
			previous->right = temp->right;
			free(temp);
			return head;
		}
		else
		{
			node* smallest = findSmallest(temp->right);
			node* smallestPrev = findPrevious(head, smallest->name);
			//smallestPrev->left = NULL;
			if(temp->right==smallest)
			{
				smallest->left = temp->left;
				previous->right = smallest;
				free(temp);
				return head;
			}
			else
			{
				smallestPrev->left=smallest->right;
				smallest->left=temp->left;
				smallest->right = temp->right;
				previous->right = smallest;
				free(temp);
				return head;
			}	

		}
	}
}

node* findSmallest(node* finder)
{
	if(finder->left!=NULL)
		return findSmallest(finder->left);
	else
		return finder;
}
