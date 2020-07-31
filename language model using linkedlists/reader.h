#ifndef READER
#define READER
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
void print_letter(float f) {
	if (f - int(f) == 0){
		if (f == 32.0) {
			cout<<"<sp>";
			return;
		}
		cout<<char(f);
		return;
	} else {
		if (f == float('c') + 0.5) {
			cout<<"ch";
		} else if (f == float('o') + 0.5) {
			cout<<"oe";
		} else if (f = float('u') + 0.5) {
			cout<<"ue";
		} else if (f = float('g') + 0.5) {
			cout<<"g~";
		} else {
			cout<<"<unk>";
		}

	}

}
class letter {	
public:
	float value;
	letter(char c) { value = float(tolower(c));}
	letter(char c1, char c2);
};

letter::letter(char c1, char c2){
	if (int(c1) > 0) {
		value = float(tolower(c1));
		return;
	}

	switch (int(c1)) {
		//ö Ö Ü ü ç Ç
		case -61 : {
			//ö Ö
			if (c2 == -74 || c2 == -106) {
				value = float('o') + 0.5;
			} /* Ü ü*/else if (c2 == -68 || c2 == -100 ) {
				value = float('u') + 0.5;
			} else {
				value = float('c') + 0.5;
			}
			break;

		}
		// ğ Ğ ı İ
		case -60 : {
			// ı İ
			if (c2 == -79 || c2 == -80 ) {
				value  = float('i');
			} else {
				value = float('g') + 0.5;
			}
			break;
		}
		//ş Ş
		case -59 : {
			value = float('s') + 0.5;
			break;
		}

	}
}

class Reader {
    const char *filename;
    vector<char> buffer;
    int index;
    int size;
public:
    Reader(const char* filename){
        this->filename = filename;
        ifstream infile(this->filename);
        char c;
        while(infile >> noskipws >> c){
        	if (c != '\n'){
        		buffer.push_back(c);
        	}
        }
        this->index = 0;
        this->size = buffer.size();
    }
    bool next(float &c){
        if (index < size){
            char c1 = buffer[index];
            if (int(c1) >= 0) {
            	index++;
            	c = letter(c1).value;
            	return true;
            } else {
            	index++;
            	c = letter(c1, buffer[index]).value;
            	index++;
            	return true;
            } 
            
        }
        index = 0;
        return false;
    }
};
#endif
