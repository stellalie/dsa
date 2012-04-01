#ifndef BINNODE_H
#define BINNODE_H

#include <math.h>
#include <iostream> 

/********************************************************\
   template node class for binary tree
\********************************************************/

template <typename dataType> class binNode 
{
   private:
      dataType nodeData;
      binNode<dataType> *left, *right;
      
      void deleteNode(binNode<dataType> **root) {
         if (left == NULL && right == NULL) {
            // leaf node
            *root = NULL;
         } else if (left == NULL) {
            // right branch but no left branch
            *root = right;
         } else if (right == NULL) {
            // left branch but no right branch
            *root = left;
         } else {
            // has left and right branch
            binNode<dataType> *temp = right;
            // find left most node of right branch
            while (temp->left != NULL) temp = temp->left;
            // attach left branch to left side of right branch
            temp->left = left;
            // make root point to right branch
            *root = right;
         }
         left = NULL;
         right = NULL;
         delete (this); 
      }
           
   public:
      
      /********************************************************\
         constructors and destructors
      \********************************************************/

      // constructors
      binNode() : left(NULL), right(NULL) {
      }
   
      binNode(const dataType& dataItem) :
         nodeData(dataItem), left(NULL), right(NULL) {
      }

      // copy constructor
      binNode(const binNode<dataType> &other) : nodeData(other.nodeData) {
          if (other.left != NULL) {
             left = new binNode<dataType>(*(other.left));
          }
          else {
             left = NULL;
          }
          if (other.right != NULL) {
             right = new binNode<dataType>(*(other.right));
          }
          else {
             right = NULL;
          }
      }
   
      // destructor
      ~binNode() {
         if (left != NULL) delete left;
         if (right != NULL) delete right;
      }
   
      /********************************************************\
         insert, delete and find
      \********************************************************/

      void insert(const dataType& dataItem) {
         if (nodeData == dataItem) {
            throw std::invalid_argument("dataItem already in tree");
         }
      
         if (dataItem < nodeData) {
            if (left == NULL) {
               left = new binNode(dataItem);
            } else {
               left->insert(dataItem);
            }
         } else {
            if (right == NULL) {
               right = new binNode(dataItem);
            } else {
               right->insert(dataItem);
            }
         }
      }
      
      void erase(binNode<dataType> **root, const dataType &delData) {
         if (delData == nodeData) {
            deleteNode(root);
         } else {
            if (delData < nodeData) {
               if (left == NULL) {
                  throw std::invalid_argument("delItem not in tree");
               } else {
                  left->erase(&left, delData);
               }
            } else {
               if (right == NULL) {
                  throw std::invalid_argument("delItem not in tree");
               } else {
                  right->erase(&right, delData);
               }
            }
         }
      }
      
      dataType* find(const dataType &findData) {
         if (findData == nodeData) {
            return &nodeData;
         } else if (findData < nodeData) {
            if (left == NULL) return NULL;
            else return left->find(findData);
         } else {
            if (right == NULL) return NULL;
            else return right->find(findData);
         }
      }

      /********************************************************\
         rebalance functions
      \********************************************************/

      bool rebalance(binNode<dataType>* &root) {
         // rebalance the subtrees hanging from this node
         int rnodes=0, lnodes=0, nodes, dif;
         bool rotate = false;
            
         if (left != NULL) {
            if (left->rebalance(left)) rotate = true;
            lnodes = left->numNodes();
         }
         if (right != NULL) {
            if (right->rebalance(right)) rotate = true;
            rnodes = right->numNodes();
         }
         
         if (rnodes > lnodes) {
            dif = rnodes - lnodes;
            // work out node difference if we rotate anticlockwise
            rnodes = 0;
            if (right != NULL && right->right != NULL) rnodes = right->right->numNodes(); 
            nodes = 0;
            if (right != NULL && right->left != NULL) nodes = right->left->numNodes();
            lnodes = 1 + lnodes + nodes;
            if (abs(lnodes - rnodes) < dif) {
               // rotate anticlockwise
               root = right;
               right = right->left;
               root->left = this;
               rotate = true;
            }
         }
         else if (lnodes > rnodes) {
            // work out node difference if we rotate clockwise
            dif = lnodes - rnodes;
            lnodes = 0;
            if (left != NULL && left->left != NULL) lnodes = left->left->numNodes();
            nodes = 0;
            if (left != NULL && left->right != NULL) nodes = left->right->numNodes();
            rnodes = 1 + rnodes + nodes;
            if (abs(lnodes - rnodes) < dif) {
               // rotate clockwise
               root = left;
               left = left->right;
               root->right = this;
               rotate = true;
            }
         }
         return rotate;
      }

      double balance() const {
         int lheight = 0, rheight = 0, dif;
         double bal = 1.0;

         if (left != NULL) {
            lheight = left->maxTreeDepth();
            bal *= sqrt(left->balance());
         }
         if (right != NULL) {
            rheight = right->maxTreeDepth();
            bal *= sqrt(right->balance());
         }

         dif = abs(lheight - rheight);
         if (dif <= 1) return bal;
         else return bal / log(dif);
      }
      
      /********************************************************\
         tree information
      \********************************************************/

      // tree statistic functions
      int maxTreeDepth() const {
         int rdepth = 0, ldepth = 0;
         
         if (left != NULL) ldepth = left->maxTreeDepth();
         if (right != NULL) rdepth = right->maxTreeDepth();
         
         if (ldepth > rdepth) return 1 + ldepth;
         else return 1 + rdepth;
      }
      
      int numNodes() const {
         int size = 1;
         
         if (left != NULL) size += left->numNodes();
         if (right != NULL) size += right->numNodes();
         
         return size;
      }
	  
	   void print() const {
	      if (left != NULL) left->print();
         std::cout << nodeData.toString() << "\n";
		   if (right != NULL) right->print();
      }
      
      int numLeafNodes() const {
         if (left == NULL && right == NULL) return 1;
         
         int numLeaf = 0;
         if (left != NULL) numLeaf += left->numLeafNodes();
         if (right != NULL) numLeaf += right->numLeafNodes();
         return numLeaf;
      }
      

      /********************************************************\
         overloaded operators
      \********************************************************/

      // overloaded dereference operator
      const dataType& operator * () const {
         return nodeData;
      }

      binNode<dataType>& operator = (const binNode<dataType> &other) {
         // remove current subtrees
         if (left != NULL) delete left;
         if (right != NULL) delete right;

         // make subtrees equal to subtree of other
         if (other.left != NULL) {
            left = new binNode<dataType>(*(other.left));
         }
         else {
            left = NULL;
         }
         if (other.right != NULL) {
            right = new binNode<dataType>(*(other.right));
         }
         else {
            right = NULL;
         }

         // make nodedata equal nodedata of other
         nodeData = other.nodeData;
      }
};

#endif
