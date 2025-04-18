#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    // void rotateLeft(AVLNode<Key, Value>* node);
    // void doubleRotateLeft(AVLNode<Key, Value>* node);
    // void rotateRight(AVLNode<Key, Value>* node);
    // void doubleRotateRight(AVLNode<Key, Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    //use same insert frim bst tree except use AVL nodes, handle balancing after
        // make the node to be inserted
    AVLNode<Key, Value>* insertion = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    if (!(this->root_)){ //if tree ie empty, make new node the root
      this->root_ = insertion;
      return;
    }
    //otherwise trverse the tree to find the parent node of the node to be inserted
    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    while(true){
      if (curr->getKey() == insertion->getKey()){ //if the key already exists, just update the value
        curr->setValue(insertion->getValue());
        delete insertion;
        return;
      }
      else if (curr->getKey() < insertion->getKey()){
        if (curr->getRight()){
          curr = curr->getRight();
        }
        else{
          curr->setRight(insertion);
          insertion->setParent(curr);
          return;
        }
      }
      else if (curr->getKey() > insertion->getKey()){
        if (curr->getLeft()){
          curr = curr->getLeft();
        }
        else{
          curr->setLeft(insertion);
          insertion->setParent(curr);
          return;
        }
      }    
    }
    // handle balances/rotation

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    //use same remove frim bst tree except use an AVL nodes, handle balancing after

    if (!(this->root_)){ // if empty tree, no nodes to be removed
      return;
    }
    AVLNode<Key, Value>* removed = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (removed){ //key exists, we should remove it
      if (removed->getLeft() && removed->getRight()){ //if boh children exist, promote predecessor
        nodeSwap(removed, static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(removed)));
      }
      AVLNode<Key, Value>* promoted = NULL;
      if ((!removed->getLeft() && removed->getRight()) || ((removed->getLeft() && !removed->getRight()))){ //one child
        if ((!removed->getLeft() && removed->getRight())){
          promoted = removed->getRight();
        }
        else{
          promoted = removed->getLeft();
        }
        if (removed->getParent()){
          if (removed->getParent()->getLeft() == removed){
            removed->getParent()->setLeft(promoted);
          }
          else{
            removed->getParent()->setRight(promoted);
          }
        }
        else{
          this->root_ = promoted;
        }
        if (promoted){
          promoted->setParent(removed->getParent());
        }
        delete removed;
        return;
      }
      else{ // 0 children
        if (removed->getParent()){
          if (removed->getParent()->getLeft() == removed){
            removed->getParent()->setLeft(NULL);
          }
          else{
            removed->getParent()->setRight(NULL);
          }
        }
        else{
          this->root_ = NULL;
        }
      }
      delete removed;
      return;
    }
    else{
      return;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
