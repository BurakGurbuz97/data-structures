#ifndef LIST
#define LIST
#include <iostream>
#include "reader.h"
#include <vector>

using namespace std;

//I will change char to read utf-8
typedef float  CHART;

//node of sublist
struct occur_node {
    CHART character;
    occur_node* next = NULL;
    int occurence = 1;
};

//node of mainlist
struct vocab_node {
    CHART character;
    int count = 1;
    vocab_node* next = NULL;
    occur_node* list = NULL;
};

//Linked list
class vocab_list {
    vocab_node* head;
    vocab_node* get_pointer(CHART c); //private methods which helps other methods
public:
    vocab_list(const char * filename);
    bool add_char(CHART c);
    void add_occurence(CHART cfirst, CHART csecond);

    int  get_union_occurence(CHART cfirst, CHART csecond);
    int  get_occurence(CHART c);

    float calculateProbability(CHART cfirst, CHART csecond);
    bool readData(const char * filename);
    void print();

};
vocab_node* vocab_list::get_pointer(CHART c) {
    vocab_node* traverse = head;
    while(traverse) {
        if (traverse->character == c ){
            return traverse;
        }
        if (traverse->next) {
            traverse = traverse->next;
        } else {
            return NULL;
        }
    }
    return NULL;
}

float vocab_list::calculateProbability(CHART cfirst, CHART csecond) {
    vocab_node* first = this->get_pointer(cfirst);
    if (first == NULL) {
        return 0;
    }
    float denominator = first->count;
    float numerator  = this->get_union_occurence(cfirst, csecond);
    return (numerator/denominator);
}
/***************** CREATION FUNCTIONS ******************/
vocab_list::vocab_list(const char * filename) {
    head = NULL;
    this->readData(filename);
}

bool vocab_list::readData(const char * filename) {
    Reader poem = Reader(filename);
    //populate main list
    CHART c;
    while(poem.next(c)){
        if (c != 12.0){
            this->add_char(c);   
        }
    }
    CHART current;
    poem.next(current);
    CHART next;
    while(poem.next(next)){
        if (next == 12.0){
            poem.next(current); 
            continue;
        }
        this->add_occurence(current, next);
        current = next;  
    }
    return true;
}
/*********************** ADD CHAR ************************************/
bool vocab_list::add_char(CHART c) {
    //First addition
    if (head == NULL) {
        vocab_node* new_node = new vocab_node;
        new_node->character = c;
        new_node->next = NULL;
        head = new_node;
        return true;
    }
    //add before head
    if (c < head->character){
        vocab_node* new_node = new vocab_node;
        new_node->character = c;
        new_node->next = head;
        head = new_node;
        return true;
    }
    //normal addition
    vocab_node* traverse = head;
    if (c == traverse->character){
        traverse->count++;
        return false;
    }
    while(traverse->next) {
        //Already exits
        if (c == traverse->next->character){
            traverse->next->count++;
            return false;
        }
        //Next
        if (c > traverse->next->character){
            traverse = traverse->next;
        } else {
            break;
        }    
    }
    //Create node
    vocab_node* new_node = new vocab_node;
    new_node->character = c;
    //End of the list
    if(traverse->next == NULL) {
        new_node->next = NULL;
        traverse->next = new_node;
        return true;
    }
    new_node->next = traverse->next;
    traverse->next = new_node;
    return true;
}
/*********************** ADD OCCURENCE ************************************/
void vocab_list::add_occurence(CHART cfirst, CHART csecond){
    vocab_node* traverse = this->get_pointer(cfirst);
    if (traverse == NULL) {
        cout<<"[INFO] add_occurence failed "<<cfirst<<" is not in mainlist"<<endl;
        return;
    }
    occur_node* tail = traverse->list;
    //Adding first node
    if(tail == NULL) {
        occur_node* new_node = new occur_node;
        new_node->character = csecond;
        new_node->next = NULL;
        traverse->list = new_node;
        return ;
    }
    //Adding before head
    if (tail->character > csecond) {
        occur_node* new_node = new occur_node;
        new_node->character = csecond;
        new_node->next = tail;
        traverse->list = new_node;
        return;
    }
    //Normal addition
    while(tail->next) {
        if (tail->character == csecond) {
            tail->occurence++;
            return;
        }
        if (tail->next->character > csecond) {
            occur_node* new_node = new occur_node;
            new_node->character = csecond;
            new_node->next = tail->next->next;
            tail->next = new_node;
            return;
        } else {
            tail = tail->next;
        }
    }
    //end addition
    if(tail->character == csecond){
        tail->occurence++;
        return;
    } else {
        occur_node* new_node = new occur_node;
        new_node->character = csecond;
        new_node->next = NULL;
        tail->next = new_node;
    }
}
/*********************** GET UNION OCCURENCE ************************************/
int vocab_list::get_union_occurence(CHART cfirst, CHART csecond) {
    vocab_node* traverse = this->get_pointer(cfirst);
    if (traverse == NULL) {
        cout<<"[INFO] get_union_occurence failed "<<cfirst<<" is not in mainlist"<<endl;
        return 0;
    }
    occur_node* tail = traverse->list;
    while(tail) {
        if (tail->character == csecond) {
            return tail->occurence;
        }
        if (tail->next) {
            tail = tail->next;
        } else {
            return 0;
        }
    }
    return 0;
}
/************************ GET OCCURENCE ******************************/
int vocab_list::get_occurence(CHART c){
    vocab_node* traverse = head;
    while(traverse){
        if (traverse->character == c)
            return traverse->count;
        if (traverse->next)
            traverse = traverse->next;
        else
            return 0;
    }
    return 0;
}
/************************ print for debugging ******************************/
//my linked list does not place special char at the end but
//print method rearranges the order and convert ' ' to <sp>
void vocab_list::print(){
    vocab_node* traverse = head;
    vector<vocab_node*> none_alpha;
    //print alpha  
    while(traverse) {
        if(traverse->character < 'a'){
            none_alpha.push_back(traverse);
             if (traverse->next) 
                traverse = traverse->next;
            else break;
            continue;
        }
        print_letter(traverse->character);
        cout<<"("<<traverse->count<<"):\n";
        occur_node* tail = traverse->list;
        while(tail) {
            cout<<"\t<";
            print_letter(tail->character);
            cout<<", "<<tail->occurence<<">\n";
            if (tail->next)
                tail = tail->next;
            else break;
        }
        if (traverse->next) 
                traverse = traverse->next;
        else break;
    }
    //none alpha chars at the end
    while(!none_alpha.empty()){
        vocab_node* traverse = none_alpha.back();
        none_alpha.pop_back();
        print_letter(traverse->character);
        cout<<"("<<traverse->count<<"):\n";
        occur_node* tail = traverse->list;
        while(tail) {
            cout<<"\t<";
            print_letter(tail->character);
            cout<<", "<<tail->occurence<<">\n";
            if (tail->next)
                tail = tail->next;
            else break;
        }
    }
}
#endif