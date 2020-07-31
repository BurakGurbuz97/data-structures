#include <iostream>
#include <fstream>
#include "grid.h"
#define BORDER_ERROR 2
#define CONFLICT_ERROR 1
#define SUCCESS 0
using namespace std;

//Converts string to int
int str_to_int(const char* s) {
    int num = 0;
    for(int i = 0; true ;i++){
        if (s[i] == '\0') {
            break;
        }
        num *= 10;
        int digit = s[i] - '0';
        num += digit;
    }
    return num;
}



int main(int argc, char** argv){
    ifstream grid_file(argv[1]);
    if(!grid_file){
         cout << "Cannot open grid file. Give filename as command line argument argv[1]"<<endl;
        return 1;
    }

    char num_of_rows[8];
    char num_of_colums[8];
    grid_file >> num_of_rows  >> num_of_colums;
    Grid grid(str_to_int(num_of_rows), str_to_int(num_of_colums));

    char type;
    char row[8];
    char column[8];
    char size[8];
    
    while(!grid_file.eof()) {
        grid_file >> type >> row >> column >> size;
        switch(type) {
            case '+':
                grid.Add_Remove_plus(type, str_to_int(row), str_to_int(column), str_to_int(size));
                break;
            case '-':
                grid.Add_Remove_minus(type, str_to_int(row), str_to_int(column), str_to_int(size));
                break;
            case '/':
                grid.Add_Remove_slash(type, str_to_int(row), str_to_int(column), str_to_int(size));
                break;
            case 'x':
                grid.Add_Remove_cross(type, str_to_int(row), str_to_int(column), str_to_int(size));
                break;
        }
    }
    grid_file.close();

    ifstream inst_file(argv[2]);
    if(!inst_file){
         cout << "Cannot open instruction file. Give filename as command line argument argv[2]"<<endl;
        return 1;
    }
    char move_type[8];
    char move_row[8];
    char move_column[8];
    char move_size[8];

    while(!inst_file.eof()){
        inst_file >> move_type >> move_row >> move_column >> move_size;
        type = move_type[2];
        switch(type) {
            case 'L':
                grid.Move('l',str_to_int(move_row),str_to_int(move_column),str_to_int(move_size));
                break;
            case 'R':
                grid.Move('r',str_to_int(move_row),str_to_int(move_column),str_to_int(move_size));
                break;
            case 'U':
                grid.Move('u',str_to_int(move_row),str_to_int(move_column),str_to_int(move_size));
                break;
            case 'D':
                grid.Move('d',str_to_int(move_row),str_to_int(move_column),str_to_int(move_size));
                break;
        }
    }
}