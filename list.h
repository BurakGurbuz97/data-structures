#include <iostream>
template <class T>
struct node {
    node<T>* next;
    node<T>* prev;
    T data;
    node() {next = NULL; data = T();}
    node(T data, node<T> * next) {this->data = data; this->next = next;}
};

template <class T>
class list {
    int size;
    node<T>* head;
public:
    list(){size = 0; head = NULL;}
    void push_back(T data); // add item at the end O(n) operation
    void push_front(T data); // add item at the start O(1) operation
    void insert(T data, int index); // add to given index O(n)
    void print(); // O(n)
    T pop_back(); // O(n)
    T pop_front();// O(1)
    T remove(int index); //O(n)
};

template <class T>
void list<T>::push_back(T data) {
    if (head == NULL) {
        head = new node<T>(data, NULL);
        size++;
        return;
    }
    
    node<T>* trav = head;
    while(trav->next != NULL)
        trav = trav->next;
    node<T> * temp = new node<T>(data, NULL);
    trav->next = temp;
    size++;
}

template <class T> 
void list<T>::push_front(T data) {
    if (head == NULL) {
        head = new node<T>(data, NULL);
        size++;
        return;
    }

    node<T> * temp = new node<T>(data,head);
    head = temp;
    size++;
}

template <class T>
void list<T>::print() {
    node<T>* trav = head;
    std::cout<<"{";
    while(trav->next != NULL) {
        std::cout<<trav->data<<", ";
        trav = trav->next;
    }
    std::cout<<trav->data<<"}"<<std::endl;
}


template <class T>
T list<T>::pop_back() {
    if(size == 0) {
        return T();
    } 
    node<T>* trav = head;
    while(trav->next->next != NULL) {
        trav = trav->next;
    }
    T to_return = trav->next->data;
    delete trav->next;
    size--;
    trav->next = NULL;
    return to_return;
}

template <class T>
T list<T>::pop_front() {
    if (size == 0) {
        return T();
    }
    node<T>* temp = head;
    head = head->next;
    T to_return = temp->data;
    delete temp;
    size--;
    return to_return;
}

template <class T>
void list<T>::insert(T data, int index) {
    if (index >= size) {
        this->push_back(data);
        return;
    }
    if (index <= 0) {
        this->push_front(data);
        return;
    }
    node<T>* trav = head;
    for(int cur_ind = 0; cur_ind++ < index - 1; trav = trav->next);
    node<T>* temp = new node<T>(data, trav->next);
    trav->next = temp;
    size++;
}

template <class T>
T list<T>::remove(int index) {
    if(index < 0 ||  index >= size)
        return T();
    if (index == 0 || index == size-1)
        return (index == 0 ? this->pop_front() : this->pop_back());
    node<T> * trav = head;
    for(int cur_ind = 0; cur_ind++ < index -1; trav = trav->next);
    node<T> * delete_this = trav->next;
    T return_this = delete_this->data;
    trav->next = delete_this->next;
    delete delete_this;
    size--;
    return return_this;
}