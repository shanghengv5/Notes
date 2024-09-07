//
//  helper.cpp
//  C++
//
//  Created by qt on 2024/3/19.
//

#include "helper.hpp"

void OpenUserFile(ifstream& input) {
     while(true) {
         cout << "Enter filename: ";
         string filename = GetLine();

         input.open(filename.c_str()); // See Chapter 3 for .c_str().
         if(input.is_open()) return;

         cout << "Sorry, I can't find the file " << filename << endl;
         input.clear();
     }
 }

string GetFileContents(ifstream& input) {
    string result;
    string line;
    
    while(getline(input, line)) {
        result += line + "\n";
    }
//    cout << "Files: " << result;
    return result;
}

void PreprocessString(string& text) {
    for (size_t k = 0; k < text.size(); ++k) {
        if(ispunct(text[k]) && text[k] != '_')  {
            text[k] = ' ';
        }
    }
}

string GetLine() {
     string result;
     getline(cin, result);
     return result;
}

int GetInteger() {
    int integer;
    while(true) {
        stringstream converter;
        converter << GetLine();
        
        if(converter >> integer) {
            char remaining;
            if (converter >> remaining) {
                cout << "Unexpected character:" << remaining << endl;
            }
            return integer;
        } else {
            cout << "Please enter an integer"  << endl;
        }
        cout << "Retry :" << endl;
    }
}
