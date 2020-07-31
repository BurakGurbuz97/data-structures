#include <iostream>

template <class T>
struct node_b {
    node_b<T> * left;
    node_b<T> * right;
    T data;
    node_b(node_b<T> * l, T dt, node_b<T> * r) {
        left = l;
        data = dt;
        right = r;
    }
};


template <class T>
class BST {
    node_b<T>* root;
    void destroy(node_b<T> *node){
        if(node) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }      
    }
    void insert_rec(T data, node_b<T> * node);
    void print_rec(int type, node_b<T> * node);
    
public:
    BST(){root = NULL;}
    ~BST(){this->destroy(root);}
    void insert(T data) {
        if (root == NULL) {
            root = new node_b<T>(NULL,data, NULL);
            return;
        }
        this->insert_rec(data,root);
    }
    // 0 = pre, 1 = in, 2 = post default is 0
    void print(int type = 0) {
        (type == 0 || type == 1 || type == 2) ?  print_rec(type, root) : print_rec(0, root);
    }
};

template <class T>
void BST<T>::insert_rec(T data, node_b<T>* node){
    if (node->data > data) {
        if(node->left) {
            insert_rec(data, node->left);
        } else {
            node->left = new node_b<T>(NULL, data, NULL);
            return;
        }
    } else if (node->data < data) {
        if(node->right) {
            insert_rec(data, node->right);
        } else {
            node->right = new node_b<T>(NULL, data, NULL);
            return;
        }
    } else {
        return;
    }
}

template <class T>
void BST<T>::print_rec(int type, node_b<T> * node) {
    if(node) {
        if (type == 0) {std::cout<<node->data<<" ";}
        print_rec(type, node->left);
        if (type == 1) {std::cout<<node->data<<" ";}
        print_rec(type, node->right);
        if (type == 2) {std::cout<<node->data<<" ";}
    }
    
}

