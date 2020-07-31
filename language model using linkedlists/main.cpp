#include <iostream>
#include <vector>
#include "list.h"

using namespace std;

int main() {
    vocab_list list = vocab_list("turkish.txt");
    list.print();
}