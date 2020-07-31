#include <iostream>

//Stack implementation which uses consecutive memory

template <class T>
class stack {
    int size;
    int capactiy;
    T* bottom;
    void extend() {
        std::cout<<"Extended "<<capactiy << " -> " << capactiy* 2<<std::endl;
        T* temp = new T[capactiy*2];
        for(int i = 0; i < size; i++)
            temp[i] = bottom[i];
        delete[] bottom;
        bottom = temp;
        capactiy = capactiy*2;
    }
    void shrink() {
        std::cout<<"Shrinked "<<capactiy << " -> " << capactiy / 2<<std::endl;
        T* temp = new T[capactiy/2];
        for(int i = 0; i < size; i++)
            temp[i] = bottom[i];
        delete[] bottom;
        bottom = temp;
        capactiy = capactiy / 2;
    }
public:
    stack(int cp = 8) {
        if (cp <= 0) {
            cp = 1;
        }
        bottom = new T[cp];
        size = 0;
        capactiy = cp;
    }
    void push(T data);
    T pop();
    bool isempty() {return (size == 0 ? true : false);}
    void print() {
        std::cout<<"#----- STACK -----#"<<std::endl;
        for(int i = size - 1; i >= 0 ; i--)
            std::cout<<bottom[i]<<std::endl;
        std::cout<<"#------ END ------#"<<std::endl;
    }
};

template <class T>
void stack<T>::push(T data){
    if (size == capactiy) 
        this->extend();
    bottom[size] = data;
    size++;
}

template <class T>
T stack<T>::pop() {
    if (size == 0) return T();
    if (size * 4 < capactiy && capactiy > 8) 
        this->shrink();
    T data = bottom[size -1];
    size--;
    return data;
}