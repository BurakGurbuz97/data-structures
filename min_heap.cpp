#include <string>
#include <iostream>
#include <fstream>

using namespace std;

struct node {
    int key;
    string data;
    bool start ;
    node(){key = 0, data = "None"; start = false;}
};

class min_heap{
    node * elems;
    int  capacity;
    int  sz;
    int get_parent(int index){return index / 2;}
    int get_left(int index){return 2 * index ;}
    int get_right(int index){return 2 * index + 1;}
    void swap(int i1, int i2);
    void fix_insert(int index);
    void fix_remove(int index);
public:
    min_heap(int capacity);
    node remove();
    void insert(node element);
    void print() 
    { 
        for (int i = 1; i <= sz / 2; i++) { 
            cout<<" PARENT : "<<elems[i].key<< " LEFT CHILD : "
                <<elems[2 * i].key<<" RIGHT CHILD :"<<elems[2 * i + 1].key<<endl;
        } 
    } 
    node get_root(){
        return elems[1];
    }
    bool empty(){return sz == 0;}
  
};

min_heap::min_heap(int capacity){
    sz = 0; this->capacity = capacity;
    elems = new node[capacity + 1];
    elems[0] = node();
}

void min_heap::swap(int i1, int i2) {
    node temp;
    temp = elems[i1];
    elems[i1] = elems[i2];
    elems[i2] = temp;
}

    
node min_heap::remove() {
    node min = elems[1];
    elems[1] = elems[sz];
    elems[sz] = node();
    fix_remove(1);
    sz--;
    return min;
}

void min_heap::insert(node element){
    if(sz == capacity) {
        cout<<"Min Heap is full!"<<std::endl;
        return;
    }
    sz++;
    elems[sz] = element;
    fix_insert(sz);
}

void min_heap::fix_insert(int index){
    int parent = get_parent(index);
    while(index > 0 && elems[parent].key > elems[index].key) { 
        swap(index, parent); 
        index = parent;
        parent = parent / 2;
    }
}

void min_heap::fix_remove(int index){
    int smallest = index; 
    int left = get_left(index);
    int right = get_right(index);
    if (left < sz && elems[smallest].key > elems[left].key)
        smallest = left;
    if (right < sz && elems[smallest].key > elems[right].key)
        smallest = right;
    if (smallest != index){
        swap(index, smallest);
        fix_remove(smallest);
    }
}


int main(int argc, char* argv[]) {
    if (argc == 1) {
        cout<<"Please specify the input file as a command line argument"<<endl;
        return 1;
    }
    char* filename = argv[1];
    ifstream infile(filename);
    if(infile.fail()){
        cout<<"cannot open: "<<filename<<endl;
        return 1;
    }
    int count = 0;
    int start;
    int end;
    string event_name;
    while(infile >> event_name >> start >> end)
        count++;
    if (count == 0)
        return 0;
    infile.clear();
    infile.seekg(0, ios::beg);

    min_heap MinHeap(count * 2);
    while(infile >> event_name >> start >> end){
        node a;
        a.data = event_name;
        a.key = start;
        a.start = true;
        MinHeap.insert(a);
        a.key = end;
        a.start = false;
        MinHeap.insert(a);
    }
    
    int T = 0;
    while(true){
        while(MinHeap.get_root().key == T){
            node event = MinHeap.remove();
            cout<<"TIME "<<T<<": "<<event.data<<" "<<(event.start ? "STARTED" : "ENDED")<<endl;
            if (MinHeap.empty())
                break;   
        }
        if (MinHeap.empty())
            break;
        T++;
    }
    return 0;
}