using namespace std;

template <class T>
class vector;

template<class T>
ostream& operator<<(ostream& os, const vector<T>& V);



template<class T>
class vector{
	int sz;
	T *elem;
	int space;
public:
	vector(); //default constructor 
	explicit vector(int s);  //constructor(size) 
	vector(std::initializer_list<T>); // initializer_list constructor
	vector(const vector&); //copy constructor
	vector& operator=(const vector&); //copy assignment
	vector(vector&& a); //move constructor
	vector& operator=(vector&& a); //move assignment
	T& operator[](int index); //subscript operator 
	const T& operator[](int index) const; //const version of subscript
	T get(int index); // gets value
	void set(int i,T val); //sets value
	void print();  //prints the vector
	void free_memo(int); //frees memory
	void reserve(int newalloc);  //allocates new space for vector;
	int capacity(){return space;}//returns 
	int size() const{return sz;} //returns size
	void push_back(T item); // push_backs item
	void resize(int newsize); // resizes vector
	void fit(); //returns empty spaces to free store 
	void pop(); //pops the last element
	void erase(int index); // erases the element at index
	void insert(int index,T val); //inserts element before the index
	long double memo_usage(); //returns memory usage of vector
	~vector(){delete[] elem;} //destructor
	friend ostream& operator<< <>(ostream& os, const vector<T>& V); //ostream operator to print the string version of object

};

///////////////////////////////////////////////////////////////////////
#include <iostream>
#include <algorithm>

template<class T> ostream& operator<<(ostream& os, const vector<T>& V){
	os<<"{";
	for(int i = 0; i<V.size()-1 ; i++){
		os<<V[i]<<", ";
	}
	os<<V[V.size()-1]<<"}"<<endl;
	return os;
}

template<class T> void vector<T>::pop(){
	if(sz <= 1){
		cout<<"Cannot pop head"<<endl;
		return;
	}
	if(sz < space/4)
		free_memo(space/2);
	*(elem+sz-1) = 0;
	sz--;
}

template<class T> vector<T>::vector(){
	sz = 0;
	elem = nullptr;
	space = 0;
}

template<class T> vector<T>::vector(int s){     
	sz = s;
	space = s;
	elem = new T [s];
	for(int i = 0; i<sz ; i++)
		elem[i] = 0.0;
}
template<class T> long double vector<T>::memo_usage(){
	return sizeof(T)*space;
}

template<class T> vector<T>::vector(initializer_list<T> lst){ 
	sz = lst.size();
	space = sz;
	elem =new T[sz];
	copy(lst.begin(),lst.end(),elem);
}

template<class T> void vector<T>::set(int index, T  val){
	if(index >= sz ){
		cout<<"Out of range"<<endl;
		return;
	}
	elem[index] = val;
}

template<class T> T  vector<T>::get(int index){
	if(index >= sz ){
		cout<<"Out of range"<<endl;
		return;
	}
	return elem[index];
}

template<class T> void vector<T>::insert(int index, T val){
	T* p = new T[sz+1];
	for(int i = 0; i<index ;i++)
		p[i] = elem[i];
	p[index] = val;
	for(int i = index+1 ; i <= sz+1; i++)
		p[i] = elem[i-1];
	delete[] elem;
	sz++;
	space = sz++;
	elem = p;
}

template<class T> void vector<T>::erase(int index){
	T* p = new T[sz-1];
	for(int i = 0; i<index; i++)
		p[i] = elem[i];
	for(int i = index+1; i < sz; i++)
		p[i-1] = elem[i];
	delete[] elem;
	sz--;
	space = sz;
	elem = p;
}

template<class T> void vector<T>::print(){
	cout<<"{";
	for(int i = 0 ; i < sz-1 ;i++){
		cout<<elem[i]<<", ";
	}
	cout<<elem[sz-1]<<"}"<<endl;
}

template<class T> vector<T>::vector(const vector& arg){
	sz = arg.sz;
	space = sz;
	elem ={new T [sz]};
	copy(arg.elem, arg.elem + sz, elem);
}

template<class T> vector<T>& vector<T>::operator=(const vector& a){
	if(this == &a)
		return *this;
	if(a.sz <= space){
		for(int i = 0; i<a.sz; i++)
			elem[i] = a.elem[i];
		sz = a.sz;
		return *this;
	}
	T * p = new T [a.sz];
	for(int i = 0; i<a.sz; i++)
		p[i] = a.elem[i];
	delete[] elem;
	elem = p;
	sz = a.sz;
	space = sz;
	return *this;
}

template<class T> vector<T>::vector(vector&& a){
	sz = a.sz;
	elem = a.elem;
	a.sz = 0;
	a.elem = nullptr;
}

template<class T> vector<T>& vector<T>::operator=(vector&& a){
	delete[] elem;
	elem = a.elem;
	sz = a.sz;
	a.elem = nullptr;
	a.sz = 0;
	return *this;
}


template<class T>  T& vector<T>::operator[](int index){
	return elem[index];
}

template<class T> const T& vector<T>::operator[](int index) const{
	return elem[index];
}

template<class T> void vector<T>::free_memo(int free){
	if(space-free < sz){
		cout<<"You will loss info if you free this much memory aborting operation ...."<<endl;
		return;
	}
	T *p = new T[space-free];
	for(int i = 0; i < sz; i++)
		p[i] = elem[i];
	delete[] elem;
	elem = p;
	space = space-free;
}

template<class T> void vector<T>::reserve(int newalloc){
	if(newalloc <= space)
		return;
	T * p = new T [newalloc];
	for(int i = 0; i<sz; i++)
		p[i] = elem[i];
	delete[] elem;
	elem = p;
	space = newalloc;
}

template<class T> void vector<T>::resize(int newsize){
	reserve(newsize);
	for(int i = sz; i<newsize; i++)
		elem[i] = T();
	space = newsize;
}

template<class T> void vector<T>::push_back(T item){
	if(space==0)
		reserve(8);
	else if (sz == space)
		reserve(2*space);
	elem[sz] = item;
	++sz;
}

template<class T> void vector<T>::fit(){
	if(space == sz)
		return;
	T* temp = (elem+sz);
	delete[] temp;
	cout<<sizeof(T)*(space-sz)<<" bytes of memory freed"<<endl;
	space = sz;
}