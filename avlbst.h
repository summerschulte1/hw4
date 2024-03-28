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
    void insertfix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n); // Corrected
    void rotateRight(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n); // Corrected
    void rotateLeft(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n); // Corrected
    void removeFix(AVLNode<Key,Value>* n, int diff);




};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO
    //if balance of parent node = +-1, then do nothing
    //if balance == 0, call insertfix

    //first, insert it into the tree and then fix the balance
    // Check if the tree is empty
    if (this->root_ == nullptr) {
        // Create the root node with the given key-value pair
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }
    
    AVLNode<Key, Value>* currentNode = static_cast<AVLNode<Key,Value>*>(this->root_);
    AVLNode<Key, Value>* parentNode = nullptr;

    // Traverse the tree to find the insert location
    while (currentNode != nullptr) {
        parentNode = currentNode;
        
        // If the new key is less than the current node's key, go left
        if (new_item.first < currentNode->getKey()) {
            currentNode = currentNode->getLeft();
        }
        // If the new key is greater than the current node's key, go right
        else if (new_item.first > currentNode->getKey()) {
            currentNode = currentNode->getRight();
        }
        // If a node with the given key already exists, update its value
        else {
            currentNode->setValue(new_item.second);
            return;
        }

    }   
    //do bst insert and change it to AVl node 
    // Link the new node to its parent
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parentNode);

    if (new_item.first < parentNode->getKey()) {
        parentNode->setLeft(newNode);
    } else {
        parentNode->setRight(newNode);
    }

    if ((newNode->getParent())->getBalance() == -1 || (newNode->getParent())->getBalance() == 1) {
        (newNode->getParent())->setBalance(0);
    }
    else if ((newNode->getParent())->getBalance() == 0) {
      //update b(p)??
      if (newNode == (newNode->getParent())->getLeft()) {
        (newNode->getParent())->setBalance(-1);
      }
      else {
        (newNode->getParent())->setBalance(1);
      }
      insertfix(newNode->getParent(), newNode);
    }
}
template<class Key, class Value>
void AVLTree<Key, Value>::insertfix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n) {
  if (p == nullptr || p->getParent() == nullptr) {
    return;
  }
  AVLNode<Key, Value>* g = static_cast<AVLNode<Key, Value>*>(p->getParent());

  if (p == g->getLeft()) { //its the left child
    //update g's balance 
    g->updateBalance(-1);

    //case 1: b(g) == 0
    if (g->getBalance() == 0) {
      return;
    }
    //case 2: b(g) == -1 -> recurse with insertfix
    if (g->getBalance() == -1) {
      insertfix(g,p);
    }
    //case 3
    if (g->getBalance() == -2) {
      //check for zig zig or zig zag
      //zig zig
      if (p->getLeft() == n) {
        rotateRight(g,p);
        p->setBalance(0);
        g->setBalance(0);
      }
      //zig zag
      else {
        rotateLeft(p,n);
        rotateRight(g,n);
        if (n->getBalance() == -1) {
          p->setBalance(0);
          g->setBalance(1);
          n->setBalance(0);
        }
        else if (n->getBalance() == 0) {
          p->setBalance(0);
          g->setBalance(0);
          n->setBalance(0);
        }
        else {
          //b(n) == 1
          p->setBalance(-1);
          g->setBalance(0);
          n->setBalance(0);
        }
      }
    }
  }
    else if (p == g->getRight()) { //its the right child
    //update g's balance 
    g->updateBalance(1);

    //case 1: b(g) == 0
    if (g->getBalance() == 0) {
      return;
    }
    //case 2: b(g) == -1 -> recurse with insertfix
    if (g->getBalance() == 1) {
      insertfix(g,p);
    }
    //case 3
    if (g->getBalance() == 2) {
      //check for zig zig or zig zag
      //zig zag
      if (p->getRight() == n) {
        rotateLeft(g, p);
        p->setBalance(0);
        g->setBalance(0);
      }
      //zig zig
      else {
        rotateRight(p,n);
        rotateLeft(g,n);
        if (n->getBalance() == 1) {
          p->setBalance(0);
          g->setBalance(-1);
          n->setBalance(0);
        }
        else if (n->getBalance() == 0) {
          p->setBalance(0);
          g->setBalance(0);
          n->setBalance(0);
        }
        else {
          //b(n) == 1
          p->setBalance(1);
          g->setBalance(0);
          n->setBalance(0);
        }
      }
    }
  }

}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n) {
  if (p->getParent() != NULL) {
    if (p == p->getParent()->getLeft()) { //keep
      p->getParent()->setLeft(n);
    }
    else {
      p->getParent()->setRight(n);

    }
  }
  else {
    this->root_ = n;
  }
  //change
  n->setParent(p->getParent());
  p->setParent(n);
  //flip
  p->setRight(n->getLeft());
  if (n->getLeft() != NULL) {
    n->getLeft()->setParent(p);

  }
  n->setLeft(p);


}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n) {
  if (p->getParent() != NULL) {
    if (p == p->getParent()->getLeft()) { //keep
      p->getParent()->setLeft(n);
    }
    else {
      p->getParent()->setRight(n);

    }
  }
  else {
    this->root_ = n;
  }
  //change
  n->setParent(p->getParent());
  p->setParent(n);
  //flip
  p->setLeft(n->getRight());
  if (n->getRight() != NULL) {
    n->getRight()->setParent(p);

  }
  n->setRight(p);
//   AVLNode<Key, Value>* c = n->getLeft();
//   AVLNode<Key, Value>* x = c->getRight();
//   AVLNode<Key, Value>* p = n->getParent();

//   if (n == p->getLeft()) {
//     p->setLeft(c);
//   }
//   else {
//     p->setRight(c);

//   }
//   c->setParent(p);
//   c->setRight(n);
//   n->setLeft(x);
//   n->setParent(c);
//   x->setParent(n);
// }
}




/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (!nodeToRemove) return;  // Key not found, nothing to remove.

    int balanceChange;
    AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(nodeToRemove->getParent());

    // If node has two children, find predecessor.
    if (nodeToRemove->getLeft() && nodeToRemove->getRight()) {
        AVLNode<Key, Value>* predecessorNode = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeToRemove));
        this->nodeSwap(nodeToRemove, predecessorNode); // You may need to cast to AVLNode in nodeSwap.
        nodeToRemove = predecessorNode;
    }

    AVLNode<Key, Value>* child = static_cast<AVLNode<Key, Value>*>(
        nodeToRemove->getLeft() ? nodeToRemove->getLeft() : nodeToRemove->getRight()
    );

    if (parent) {
        balanceChange = (parent->getLeft() == nodeToRemove) ? 1 : -1;
        if (nodeToRemove == parent->getLeft()) {
            parent->setLeft(child);
        } else {
            parent->setRight(child);
        }
    } else {
        this->root_ = child;  // nodeToRemove was the root
        balanceChange = 0;    // If we're removing the root, there's no balance change to propagate.
    }

    if (child) {
        child->setParent(parent);
    }

    // Delete the node and propagate the balance change up the tree.
    delete nodeToRemove;
    this->removeFix(parent, balanceChange);
}

template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key,Value>* n, int diff) {
  if (n == nullptr) {
    return;
  }
  //Let p = parent(n) and if p is not NULL let ndiff (nextdiff) = +1 if n is a left child and -1 otherwise
  AVLNode<Key, Value>* p = n->getParent();
  int ndiff;
  if (p != NULL && n == p->getLeft()) {
     ndiff =1;
  }
  else if (p != NULL && n == p->getRight()) {
    ndiff = -1;
  }
  //Assume diff = -1 and follow the remainder of this approach, mirroring if diff = +1
  if (diff == -1) {
    //case 1: b(n) + diff == -2
    if (n->getBalance() + diff == -2) {
      AVLNode<Key, Value>* c = n->getLeft();
      //zig zig case 1a
      if (c->getBalance() == -1) {
        rotateRight(n,c);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p,ndiff);
      }
      //1b
      if (c->getBalance() == 0) {
        rotateRight(n,c);
        n->setBalance(-1);
        c->setBalance(1);
      }
      //1c zig zag
      if (c->getBalance() == 1) {
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c,g);
        rotateRight(n,c);
        if (g->getBalance() == 1) {
          n->setBalance(0);
          c->setBalance(-1);
          g->setBalance(0);
        }
        else if (g->getBalance() == 0) {
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if (g->getBalance() == -1) {
          n->setBalance(1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p,ndiff);
      }
    }
    //case 2
    if (n->getBalance() + diff == -1) {
      n->setBalance(-1);
      return; //?
    }
    //case 3
    else if (n->getBalance() +diff == 0) {
      n->setBalance(0);
      removeFix(p,ndiff);
    }

  }
  else if (diff == 1) {
    //case 1: b(n) + diff == -2
    if (n->getBalance() + diff == 2) {
      AVLNode<Key, Value>* c = n->getLeft();
      //zig zig case 1a
      if (c->getBalance() == 1) {
        rotateLeft(n,c);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p,ndiff);
      }
      //1b
      if (c->getBalance() == 0) {
        rotateLeft(n,c);
        n->setBalance(1);
        c->setBalance(-1);
      }
      //1c zig zag
      if (c->getBalance() == -1) {
        AVLNode<Key, Value>* g = c->getLeft();
        rotateRight(c,g);
        rotateLeft(n,c);
        if (g->getBalance() == -1) {
          n->setBalance(0);
          c->setBalance(1);
          g->setBalance(0);
        }
        else if (g->getBalance() == 0) {
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if (g->getBalance() == 1) {
          n->setBalance(-1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p,ndiff);
      }
    }
    //case 2
    if (n->getBalance() + diff == 1) {
      n->setBalance(1);
      //return; //?
    }
    //case 3
    else if (n->getBalance() +diff == 0) {
      n->setBalance(0);
      removeFix(p,ndiff);
    }

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
