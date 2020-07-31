#include <iostream>

// queue implementation which uses linked list to manage memory

template <class T>
struct node_q {
    node_q<T> * next;
    T data;
    node_q(T data = T(), node_q<T> * next = NULL) {this->data = data; this->next = next;}
};


template <class T>
class queue {
    int size;
    node_q<T> * head;
    node_q<T> * tail;
public:
    queue(){head == NULL; tail = NULL; size = 0;}
    void enqueue(T data);
    T dequeue();
    bool isempty(){ return  (size == 0 ? true : false);}
    void print();
};

template <class T> 
void queue<T>::enqueue(T data) {
    node_q<T> * temp = new node_q<T>(data, NULL);
    if (tail ==  NULL) {
        tail = temp; head = tail; size = 1;
        return;
    }
    tail->next = temp;
    size ++;
    tail = tail->next;
}

template <class T>
T queue<T>::dequeue() {
    if(head == NULL) {
        return T();
    }

    node_q<T> * temp = head;
    T data = temp->data;
    head = head->next;
    delete temp;
    size--;
    return data;
}

template <class T>
void queue<T>::print(){
    if (this->isempty()){std::cout<<"EMPTY"<<std::endl; return;}
    std::cout<<"START: ";
    node_q<T> * trav = head;
    while(trav->next != NULL){
        std::cout<<trav->data<<" -> ";
        trav = trav->next;
    }
    std::cout<<trav->data<<" :END"<<std::endl;
}