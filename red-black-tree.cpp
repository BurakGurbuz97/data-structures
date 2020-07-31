/*
    AA HW3
    Mustafa Burak Gurbuz, 150150082
    mustafaburakgurbuz@gmail.com
*/

/*
1. I built red-black trees using box sizes as the node key because I want to efficiently search for the smallest boxes.

2. Since sizes are not unique, I added a priority queue data structure in each node of the tree
    so that a node can hold more than one packages and keep track of their ordinal numbers. 

3. Homework indicates "nth smallest package/s " (rule k, not the smallest size shipment) 
    however, my implementation also keeps tracks of the ordinal numbers so there is only 
    one nth smallest package (if sizes are equal, it checks ordinal numbers to determine the smallest).

4. Since my nodes can hold more than one package number of red nodes and black nodes may not add up to package count. 
    (You said it won't be a problem via email)

5. You can call RedBlackTree::print() function to view the tree. If you set the VERBOSE variable at the "hw3" function to true,
    it will print all the actions to the screen.
*/

#include <iostream>
#include <string>
#include "hw3.h"

//I used priority queue in each node to keep track of the ordinal numbers
//I used STL . In case STL is not allowed you may find my implementation here  below
//https://github.com/BurakGurbuz97/vector_for_cpp/blob/master/vector.h
//https://github.com/BurakGurbuz97/DataStructures/blob/master/queue.h
#include <vector>
#include <queue>

#define BLACK 0
#define RED 1
using namespace std;



//Print function for vector
template<class T>
void print_vec(vector<T> const& v) {
    for(auto const &i : v) cout<<i<<" "; 
    cout<<endl;
}


//Packages are not same with tree nodes
struct Package {
    int size;
    int ordinal;

    Package() {
        size = ordinal = 0;
    }

    Package(int sz, int ord) {
        size = sz;
        ordinal = ord;
    }

    //Overload <<  for package so that  print_vec(vector<T> const& v) can work on package
    friend ostream& operator<<(std::ostream& os, const Package& p) {
        os<<"("<<p.size<<", "<<p.ordinal<<")";
        return os;
    }
};

struct Node {
  int data;
  Node *parent;
  Node *left;
  Node *right;
  //greater<int> since I want smaller to be the top
  priority_queue<int,  vector<int>, greater<int>> ordinals;
  int color;

  Node(int k, int ord) {
        data  = k;
        ordinals.push(ord);
        color = RED;
        left = right = parent = NULL;
  }
};

class RedBlackTree {
    Node* root;
    //Null node which represents the leaves
    Node* TNULL;

    //Temp attributes USE THEM WITH EXTRA ATTENTION!!!
    Package found_smallest_k;
    int red_count;
    int black_count;
    int package_counter;
    int total_number_of_packages;

    //Fix RBT constraint after deletion
    void deleteFix(Node* x);
    //Exchange nodes
    void rbTransplant(Node* u, Node* v);
    //Deletes node with the given key
    void deleteNodeHelper(Node* node, int key);
    //Return left most node
    Node* minimum(Node* node);
    //Fix RBT constraint after insertion
    void insertFix(Node* k);
    //Recursive print function used by a public function
    void printHelper(Node* root, string indent, bool last);
    //Recursively count red nodes  called by a public funtion
    void count_red(Node *n);
    //Recursively count black nodes  called by a public funtion
    void count_black(Node *n);
    //Find kth smallest package
    void find_k(Node *n);
    //Recursivelt count packages
    void inorder_count_packages(Node *n);
public:
    RedBlackTree();
    //print the tree
    void print() {printHelper(root, "   ", false);}
    //Rotate tree
    void leftRotate(Node* x);
    void rightRotate(Node* x);

    //Insert give key according to ordinal number
    void insert(int key, int ord);

    int redCount() {red_count = 0; count_red(root); return red_count;}
    int blackCount() {black_count = 0; count_black(root); return black_count;}
    void deleteNode(int data) {deleteNodeHelper(this->root, data);}
    void printTree() {if (root) printHelper(this->root, "", true);}

    //Find smallest kth package
    Package k_th_package(int k) {package_counter = k; found_smallest_k = Package(0,0); find_k(root); return found_smallest_k;}
    //Get all packages with the smallest size
    vector<Package> get_smallest_packages();
    bool is_empty(){return root == TNULL;}
    int count_packages(){total_number_of_packages = 0; inorder_count_packages(root) ; return total_number_of_packages;}
};

void RedBlackTree::inorder_count_packages(Node *n) {
    if (n == TNULL)
        return;
    inorder_count_packages(n->left);
    total_number_of_packages += n->ordinals.size();
    inorder_count_packages(n->right);
}

void RedBlackTree::find_k(Node * n) {
    if (n == TNULL) {
        return;
    }
    find_k(n->left);
    if (package_counter <= -9999)
       return;
    if (package_counter - (int)n->ordinals.size() < 1) {
        priority_queue<int,  vector<int>, greater<int>> ordinals_copy;
        Package pack(0,0);
        int len = n->ordinals.size();
       
        for(int i = 0 ; i < len; i++) {
            if (i+1 == package_counter) {
                int ord = n->ordinals.top();
                n->ordinals.pop();
                pack.size = n->data;
                pack.ordinal = ord;
            } else {
                int ord = n->ordinals.top();
                n->ordinals.pop();
                ordinals_copy.push(ord);
            }
        }
        n->ordinals = ordinals_copy;
        if(n->ordinals.size() == 0){
            this->deleteNode(n->data);
        }
        found_smallest_k = pack;
        package_counter = -9999;
        return;
    } else {
        package_counter = package_counter - n->ordinals.size();
    }
    find_k(n->right);
}

vector<Package> RedBlackTree::get_smallest_packages(){
    Node * temp = root;
    if (temp  == NULL) {
        vector<Package> p;
        return p;
    }
    while(temp ->left != TNULL) {
        temp  = temp ->left;
    }
    vector<Package> p;
    int size = temp ->ordinals.size();
    for(int i = 0 ; i < size ; i++){
        int ord = temp->ordinals.top();
        temp->ordinals.pop();
        p.push_back(Package(temp ->data, ord));
    }
    this->deleteNode(temp->data);
    
    return p;
}

void RedBlackTree::deleteFix(Node* x){
    Node* s;
    while (x != root && x->color == BLACK){
      if (x == x->parent->left) {
        s = x->parent->right;
        if (s->color == RED) {
          s->color = BLACK;
          x->parent->color = RED;
          leftRotate(x->parent);
          s = x->parent->right;
        }
        if (s->left->color == BLACK && s->right->color == BLACK) {
          s->color = RED;
          x = x->parent;
        }
        else {
          if (s->right->color == BLACK) {
            s->left->color = BLACK;
            s->color = RED;
            rightRotate(s);
            s = x->parent->right;
          }
          s->color = x->parent->color;
          x->parent->color = BLACK;
          s->right->color = BLACK;
          leftRotate(x->parent);
          x = root;
        }
      }
      else
      {
        s = x->parent->left;
        if (s->color == RED) {
          s->color = BLACK;
          x->parent->color = RED;
          rightRotate(x->parent);
          s = x->parent->left;
        }
        if (s->right->color == BLACK && s->right->color == BLACK) {
          s->color = RED;
          x = x->parent;
        }
        else {
          if (s->left->color == BLACK) {
            s->right->color = BLACK;
            s->color = RED;
            leftRotate(s);
            s = x->parent->left;
          }
          s->color = x->parent->color;
          x->parent->color = BLACK;
          s->left->color = BLACK;
          rightRotate(x->parent);
          x = root;
        }
      }
    }
    x->color = BLACK;
}

void RedBlackTree::count_red(Node *n) {
    if (n == TNULL)
        return;
    count_red(n->left);
    if (n->color == RED) {
        red_count += 1;
    }
    count_red(n->right);
}

void RedBlackTree::count_black(Node *n) {
    if (n == TNULL)
        return;
    count_black(n->left);
    if (n->color == BLACK) {
        black_count += 1;
    }
    count_black(n->right);
}

void RedBlackTree::rbTransplant(Node* u, Node* v){
    if (u->parent == nullptr)
      root = v;
    else if (u == u->parent->left)
      u->parent->left = v;
    else
      u->parent->right = v;
    v->parent = u->parent;
}

void RedBlackTree::deleteNodeHelper(Node* node, int key){
     Node* temp = root;
        while(true){
            if (temp == NULL){
                break;
            } else {
                if (temp->data == key) {
                    if (temp->ordinals.size() < 2) {
                        break;
                    } else {
                        temp->ordinals.pop();
                        return;
                    }
                } else if (temp->data < key) {
                    temp = temp->right;
                } else {
                    temp = temp->left;
                }
             }
        }
    Node* z = TNULL;
    Node* x;
    Node* y;
    while (node != TNULL){
      if (node->data == key)
        z = node;
      if (node->data <= key)
        node = node->right;
      else
        node = node->left;
    }
    if (z == TNULL) {
      return;
    }
    y = z;
    int y_original_color = y->color;
    if (z->left == TNULL) {
      x = z->right;
      rbTransplant(z, z->right);
    }
    else if (z->right == TNULL) {
      x = z->left;
      rbTransplant(z, z->left);
    }
    else {
      y = minimum(z->right);
      y_original_color = y->color;
      x = y->right;
      if (y->parent == z)
        x->parent = y;
      else {
        rbTransplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }
      rbTransplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }
    delete z;
    if (y_original_color == BLACK)
      deleteFix(x);
}

Node* RedBlackTree::minimum(Node* node){
    while (node->left != TNULL)
      node = node->left;
    return node;
}

void RedBlackTree::insertFix(Node* k){
    Node* u;
    while (k->parent->color == RED)
    {
      if (k->parent == k->parent->parent->right)
      {
        u = k->parent->parent->left;
        if (u->color == RED) {
          u->color = BLACK;
          k->parent->color = BLACK;
          k->parent->parent->color = RED;
          k = k->parent->parent;
        }
        else {
          if (k == k->parent->left) {
            k = k->parent;
            rightRotate(k);
          }
          k->parent->color = BLACK;
          k->parent->parent->color = RED;
          leftRotate(k->parent->parent);
        }
      }
      else {
        u = k->parent->parent->right;
        if (u->color == RED)
        {
          u->color = BLACK;
          k->parent->color = BLACK;
          k->parent->parent->color = RED;
          k = k->parent->parent;
        }
        else {
          if (k == k->parent->right) {
            k = k->parent;
            leftRotate(k);
          }
          k->parent->color = BLACK;
          k->parent->parent->color = RED;
          rightRotate(k->parent->parent);
        }
      }
      if (k == root)
        break;
    }
    root->color = BLACK;
}

void RedBlackTree::printHelper(Node* root, string indent, bool last){
    if (root != TNULL) {
      cout << indent;
      if (last){
        cout << "R----";
        indent += "   ";
      }
      else
      {
        cout << "L----";
        indent += "|  ";
      }
      string sColor = root->color ? "RED" : "BLACK";
      cout << root->data << "(" << sColor << ")" << "(" << root->ordinals.size() <<")"<< endl;
      printHelper(root->left, indent, false);
      printHelper(root->right, indent, true);
    }
}

RedBlackTree::RedBlackTree(){
    TNULL = new Node(0,0);
    TNULL->color = BLACK;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;
}

void RedBlackTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != TNULL)
      y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
      this->root = y;
    else if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != TNULL)
      y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
      this->root = y;
    else if (x == x->parent->right)
      x->parent->right = y;
    else
      x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void RedBlackTree::insert(int key, int ord) {
    Node* node = new Node(key, ord);
    node->parent = nullptr;
    node->left = TNULL;
    node->right = TNULL;
    node->color = RED;
    Node* y = nullptr;
    Node* x = this->root;
    while (x != TNULL){
        y = x;
        if (node->data < x->data)
            x = x->left;
        //Normally it inserts same value to the right I added this lines
        else if (node->data == y->data) {
          y->ordinals.push(ord);
          node = y;
          return;
        }
        else
            x = x->right;
    }
    node->parent = y;
    if (y == nullptr)
        root = node;
    else if (node->data < y->data)
        y->left = node;
    
    else
        y->right = node;

    if (node->parent == nullptr){
        node->color = BLACK;
        return;
    }
    if (node->parent->parent == nullptr)
        return;
    insertFix(node);
}


HW3_Result hw3(int eastWarehousePackageCount, int eastWarehousePackageSizes [], int eastWarehousePackageOrdinals [],
                int westWarehousePackageCount, int westWarehousePackageSizes [], int westWarehousePackageOrdinals [] ) {
    
    //Set this to true to see the list of all package transfers 
    const bool VERBOSE = false;

    //Insert given packages to warehouses
    HW3_Result result;
    RedBlackTree East;
    for(int i = 0  ; i < eastWarehousePackageCount; i++) {
        East.insert(eastWarehousePackageSizes[i], eastWarehousePackageOrdinals[i]);
    }
    RedBlackTree West;
    for(int i = 0  ; i < westWarehousePackageCount; i++) {
        West.insert(westWarehousePackageSizes[i], westWarehousePackageOrdinals[i] );
    }


    
    //this vector represents carried packages
    vector<Package> packs = {Package(0,0)};

    //Start the transfer logic
    while(true) {
        //East action
        //Check if there is any packages waiting 
        if(packs[0].size != 0) {
            Package pack_temp = East.k_th_package(packs[0].ordinal);
            //Are we able to send kth smallest?
            //Send package and accept incomings
            if (pack_temp.size != 0) {
                if (VERBOSE) {
                    cout<<"East accepts: "<<endl<<"\t";
                    print_vec(packs);
                }
    
                for(unsigned int i = 0; i < packs.size(); i++) {
                    East.insert(packs[i].size, packs[i].ordinal);
                }

                //Send kth smallest
                vector<Package> v;
                v.push_back(pack_temp);
                packs = v;
                if (VERBOSE) {
                    cout<<"East sends: "<<endl<<"\t";
                    print_vec(packs);
                }
            } else {
                //Sent packages to smaller warehouses (do nothing packges will just disappear since they are not inserted)
                if (VERBOSE) {
                    cout<<"Packages sent to smaller warehouses:"<<endl<<"\t";
                    print_vec(packs);
                }
                
                goto EAST_SMALLEST;
            }
 
        } 
        else {
EAST_SMALLEST:
            //Prepare smallest packages shipment
            packs = East.get_smallest_packages();
            if (VERBOSE) {
                cout<<"East sends: "<<endl<<"\t";
                print_vec(packs);
            }
            //If empty we are done
            if (East.is_empty()) {
                if (VERBOSE) {
                    cout<<"West accepts: "<<endl<<"\t";
                    print_vec(packs);
                }
                goto EMPTY;
            }
        }
        
        //West action (same logic as above, just changed East to West)
        if(packs[0].size != 0) {
            Package pack_temp = West.k_th_package(packs[0].ordinal);
            if (pack_temp.size != 0) {
                if (VERBOSE) {
                    cout<<"West accepts: "<<endl<<"\t";
                    print_vec(packs);
                }
                for(unsigned int i = 0; i < packs.size(); i++) {
                    West.insert(packs[i].size, packs[i].ordinal);
                }
                vector<Package> v;
                v.push_back(pack_temp);
                packs = v;
                if (VERBOSE) {
                    cout<<"West sends: "<<endl<<"\t";
                    print_vec(packs);
                }
                
            } else {
                if (VERBOSE) {
                    cout<<"Packages sent to smaller warehouses:"<<endl<<"\t";
                    print_vec(packs);
                }
                goto WEST_SMALLEST;

            }
 
        } 
        else {
WEST_SMALLEST:
            packs = West.get_smallest_packages();
            if (VERBOSE) {
                cout<<"West sends: "<<endl<<"\t";
                print_vec(packs);
            }
            if (West.is_empty()) {
                if (VERBOSE) {
                  cout<<"East accepts: "<<endl<<"\t";
                  print_vec(packs);
                }
                
                goto EMPTY;
            }
        }
        

    }


//Done
EMPTY:
    if (West.is_empty()) {
        //West is empty so East will accept all packages
        for(unsigned int i = 0 ; i < packs.size(); i++) {
            East.insert(packs[i].size, packs[i].ordinal);
        }
        if (VERBOSE) {
            cout<<"Non-empty warehouse EAST:"<<endl;
            East.print();
        }
        result.blackNodeCount =  East.blackCount();
        result.redNodeCount   =  East.redCount();
        result.packageCount   =  East.count_packages();


    } else {
        //East is empty so West will accept all packages
        for(unsigned int i = 0 ; i < packs.size(); i++) {
            West.insert(packs[i].size, packs[i].ordinal);
        }
        if (VERBOSE) {
            cout<<"Non-empty warehouse WEST:"<<endl;
            West.print();
        }
        result.blackNodeCount =  West.blackCount();
        result.redNodeCount   =  West.redCount();
        result.packageCount   =  West.count_packages();
    }

    if (VERBOSE) {
        cout<<"Final Results: "<<endl;
        cout<<"Package Count = "<<result.packageCount<<endl;
        cout<<"Red Node Count = "<<result.redNodeCount<<endl;
        cout<<"Black Node Count = "<<result.blackNodeCount<<endl;
    }
    return result;
}


int main() {
  
}