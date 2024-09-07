//
//  helper.hpp
//  C++
//
//  Created by qt on 2024/3/19.
//

#ifndef helper_hpp
#define helper_hpp

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

void OpenUserFile(ifstream& input);
string GetFileContents(ifstream& input);
string GetLine();
int GetInteger();
void PreprocessString(string& text);
    

#endif /* helper_hpp */
