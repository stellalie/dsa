#ifndef BINTREE_H_
#define BINTREE_H_

#include <stdexcept>
#include <math.h>

#include "binnode.h"

/********************************************************\
   template class for a binary tree
\********************************************************/
      

template <typename dataType> class bintree
{  
   private:
      binNode<dataType> *root;
      int numItems;
      
   public:
      /*******************************************************\
         constructors & destructors
      \*******************************************************/
      
      // constructor
      bintree() : root(NULL), numItems(0) {}

      // copy constructor
      bintree(const bintree<dataType> &other) : numItems(other.numItems) {
         if (other.root != NULL) {
            root = new binNode<dataType>(*(other.root));
         }
         else {
            root = NULL;
         }
      }
      
      // destructor
      ~bintree() {
         if (root != NULL) delete root;
      }
      
      /*******************************************************\
         misc functions
      \*******************************************************/
      
      bool empty() const {
         return (numItems == NULL);
      }
      
      int size() const {
         return numItems;
      }
      
      int numNodes() const {
         if (root == NULL) {
            return 0;
         } else {
            return root->numNodes();
         }
      }
      
      int maxTreeDepth() const {
         if (root == NULL) {
            return 0;
         } else {
            return root->maxTreeDepth();
         }
      }
      
      int numLeafNodes() const {
         if (root == NULL) {
            return 0;
         } else {
            return root->numLeafNodes();
         }
      }
      
      double balance() const {
         // Returns a measure of how balanced a tree is.
         // A value of 1 is a prefectly balanced tree.
         // The closer to 0 the value is the more unbalanced
         // the tree.
         // A value < 0.5 indicates a tree that is seriously unbalanced
         
         if (numItems == 0) return 1.0;
         else return root->balance();
      }
      
      void rebalance() {
         // Rebalance tree using the AVL algorithm
         // While this does not guarantee a perfectly balanced tree
         // it does make it mostly balanced by guranteeing that the diference
         // in depth between every right and left subtree is at most 1
         
         if (root != NULL) {
            while (root->rebalance(root));
         }
      }
      
      /*******************************************************\
         insertion and erasure functions
      \*******************************************************/
      
      void insert(const dataType& newData) {
         if (root == NULL) {
            root = new binNode<dataType>(newData);
         } else {
            root->insert(newData);
         }
         numItems++;
      }
      
      void erase(const dataType& delData) {
      
         if (root == NULL) {
            throw std::invalid_argument("data does not exist in tree to erase");
         }
         
         root->erase(&root, delData);
         
         numItems--;
      }
      
      dataType* find(const dataType &findData) const {
         // this function looks for findData in the tree.
	 // If it finds the data it will return the address of the data in the tree
	 // otherwise it will return NULL
         if (root == NULL) return NULL;
         else return root->find(findData);
      }
	  
      /*******************************************************\
         print function for assignment. 
         assumes dataType has print() function 
      \*******************************************************/
	  
      void print() const {
          if (root != NULL) root->print();
      }

      /*******************************************************\
         overloaded operators 
      \*******************************************************/

      bintree<dataType>& operator = (const bintree<dataType> &other) {
         if (root != NULL) {
            delete root;
            numItems = 0;
         }
         if (other.root != NULL) {
            root = new binNode<dataType>(*(other.root));
            numItems = other.numItems;
         }
         return *this;
      }
};

#endif
