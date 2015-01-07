/**
 * @file
 * @brief This header delared the structure binarySearTree
 * The Tree is used by a server to manipulate data and performs a number of
 * storage operations.
*/

#ifndef BST_H
#define BST_H

#define MAX_KEY_LEN 20
#define MAX_COLUMNS_PER_TABLE 10
#define MAX_STRTYPE_SIZE 40

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> 


typedef struct binarySearchTree{
	char name[20];
	char population[MAX_COLUMNS_PER_TABLE] [MAX_STRTYPE_SIZE];
	struct binarySearchTree* left;
	struct binarySearchTree* right;
}node;

node* createNode(char*name,char population [MAX_COLUMNS_PER_TABLE] [MAX_STRTYPE_SIZE]); //works
node* insert(node* head, char* name,char population[MAX_COLUMNS_PER_TABLE] [MAX_STRTYPE_SIZE]); //works
node* get(node*head, char*name,char* value[]); //works
node* set(node*head, char*name, char population[MAX_COLUMNS_PER_TABLE] [MAX_STRTYPE_SIZE]); //works
node* findPrevious(node* previous, char* name_);
node* deleteNode(node*head, char*name_);

bool checkParameters(node* head, char parameters[MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE+4], const int numParams);
void BSTQuery(node* head, char parameters[MAX_COLUMNS_PER_TABLE][MAX_STRTYPE_SIZE+4], const int max_keys, int* numKeys, char keys[max_keys][MAX_KEY_LEN], const int numParams);

node* findSmallest(node* finder); // works

node* deleteAll(node* head); //seems to work, like there's nothing there. The absence of evidence seems to support the evidence of absence.

void printAll(node* head);// works

#endif
