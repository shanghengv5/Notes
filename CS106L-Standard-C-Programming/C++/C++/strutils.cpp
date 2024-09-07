//
//  strutils.cpp
//  C++
//
//  Created by qt on 2024/3/4.
//

#include "strutils.hpp"
#include <cctype>
#include <sstream>

string ConvertToUpperCase(string input) {
     for (size_t k = 0; k < input.size(); ++k)
     input[k] = toupper(input[k]);
     return input;
}
string ConvertToLowerCase(string input) {
     for (size_t k = 0; k < input.size(); ++k)
     input[k] = tolower(input[k]);
     return input;
}
string IntegerToString(int input) {
     stringstream converter;
     converter << input;
     return converter.str();
}
string DoubleToString(double input) {
     stringstream converter;
     converter << input;
     return converter.str();
}
