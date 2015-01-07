#ifndef _TREENODE_H
#define _TREENODE_H

#include "DBentry.h"

class TreeNode {
private:
    DBentry* entryPtr;
    TreeNode* left;
    TreeNode* right;
	
	//A RECURSIVE FIND MAXIMUM ENTRY BENEATH A NODE FUNCTION
	DBentry* findMax();
public:
    // A useful constructor
    TreeNode(DBentry* _entryPtr);

    // the destructor
    ~TreeNode();
			
	//RECURSIVE DESTRUCTOR
	void deleteNode(string name_,TreeNode* &ptr);
	
	//A recursive clear-all destructor
	void deleteAll();
	
    // sets the left child of the TreeNode.
    void setLeft(TreeNode* newLeft);

    // sets the right child of the TreeNode
    void setRight(TreeNode* newRight);

    // gets the left child of the TreeNode.
    TreeNode* getLeft() const;

    // gets the right child of the TreeNode
    TreeNode* getRight() const;

    // returns a pointer to the DBentry the TreeNode contains. 
    DBentry* getEntry() const;
};

#endif 
