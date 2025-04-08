#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int numEdges(Node * root); //helper function that does the work for equal paths



bool equalPaths(Node * root)
{
    //return result of helper function
    if (!root){
        return -1;
    }
    return numEdges(root) != -1;
}

int numEdges (Node * root){
    //similar to isBalanced fro lab 8, use that as foundation
    if (!root){
        return -1; //error flag
    }
    if (!root->left && !root->right){ //leaf node, no edges
        return 0;
    }
    int lsd = numEdges(root->left); //get left subtree depth
    int rsd = numEdges(root->right); //get right subtree depth
    if (root->left && root->right){ // check if both children exist then see if they match
        if (lsd != rsd){
            return -1; //pass back up an error flag to show we found two nonequal paths
        }
    }
    if (max(lsd, rsd) != -1){  // check if at least one subtree has a valid depth
        return max(lsd, rsd) + 1; // just take the depth one valid subtree
    }
    else{
        return -1; //return error flag to indicate none of the subtrees have a valid depth
    }
}

