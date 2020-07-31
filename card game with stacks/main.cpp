#include <iostream>
#include <fstream>
using namespace std;
// Since dividing code to headers is not an option
// I wrote all methods inline to avoid spaghetti code

//Node for Stack 
struct Node {
    Node* bottom;
    int value;
    Node(Node* btm, int val) {bottom = btm; value = val;}
};
//Implementation of stack<int> uses linkedlist structure to manipulate data
class Stack {
    Node* top;
    int size;
public:
    Stack(){top = NULL; size = 0;}
    ~Stack(){
        Node* temp = NULL;
        while(top){
            temp = top;
            top = top->bottom;
            delete temp;
        }
    }
    void print() {
        Node* traversa = top;
        while(traversa){
            cout<<traversa->value<<endl;
            traversa = traversa->bottom;
        }
    }
    int pop() {
        int val = int();
        if (!this->empty()) {
            val = top->value;
            Node* temp = top;
            top = temp->bottom;
            delete temp;
            size--;
        }
        return val;
    }
    void push(int val) {
        Node* new_node = new Node(top,val);
        top = new_node;
        size++;
    }
    bool empty() {return (size == 0 ? true : false);}
    //See the top of the stack without pushing or popping
    int peek_top() { 
        if(size == 0) return int();
        return top->value;
    }
    int get_size(){ return size; }
};
//Class to simulate game envriment
class CardGame {
   Stack playerOne;
   Stack playerTwo;
   Stack desk;
   Stack bin;
public:
    //Constructor which takes filename and distrubite card according to rules
    //Fails if input is wrong
    CardGame(const char* filename){
        ifstream inFile;
        inFile.open(filename);
        if (!inFile) {
            cout << "Unable to open file "<< filename<<endl;
            return;
        }
        int desk_count, player_deck_count;
        inFile >> desk_count >> player_deck_count;
        if (desk_count < 0 || player_deck_count < 0){
            cout << "Deck count cannot be negative "<< filename<<endl;
            return;
        }
        for (int i = 0; i < desk_count ; i++ ){
            int x;
            if (!(inFile >> x)) {
                cout << " Invalid Input "<< filename<<endl;
                return;
            }
            desk.push(x);
        }
        for (int i = 0; i < player_deck_count ; i++ ){
            int x;
            if (!(inFile >> x)) {
                cout << " Invalid Input "<< filename<<endl;
                return;
            }
            playerOne.push(x);
        }
        for (int i = 0; i < player_deck_count ; i++ ){
            int x;
            if (!(inFile >> x)) {
                cout << " Invalid Input "<< filename<<endl;
                return;
            }
            playerTwo.push(x);
        }
    }
    //simulate one turn takes boolean if it is p1's turn or not
    //It may seem cryptic since I use some tricks to avoid repetitive
    bool playTurn(bool p1_plays){
        //Check if game ended
        if (playerOne.empty() || playerTwo.empty() || desk.empty()) return false;
        //Trick to shorten code (switch players if card positive)
        int drawed_card = desk.pop();
        if (drawed_card > 0)
            p1_plays = !p1_plays;
        else
            drawed_card = -drawed_card;
        //Alias players to avoid repeatative code
        Stack *player = p1_plays ? &playerOne : &playerTwo ;
        Stack *other  =  p1_plays ? &playerTwo : &playerOne ;
        for(int i = 0 ; i < drawed_card; i++) {
            int card_to_be_taken = player->pop();
            if (card_to_be_taken >  other->peek_top())
                other->push(card_to_be_taken);
            else 
                bin.push(card_to_be_taken);
            if (player->empty()){
                return false;
            }
        }
        return true;     
    }
    //calls playTurn until game ends
    void simulateGame(){
        bool p1_plays = true;
        while(this->playTurn(p1_plays)) p1_plays = !p1_plays;
        cout<<bin.get_size()<<endl;
    }
};
//Creates game object and plays one game
int main(int, char **argv) {
    CardGame cGame(argv[1]);
    cGame.simulateGame();
}