//
//  x-macro.cpp
//  C++
//
//  Created by qt on 2024/3/11.
//

#include "x-macro.hpp"
using namespace std;

Color GetOppositeColor(Color c) {
    switch (c) {
        #define DEFINE_COLOR(color, opposite) case cName(color): return cName(opposite);
        #include "color.h"
        #undef DEFINE_COLOR
        default: return c;
    };
}

string ColorToString(Color c) {
    switch (c) {
    #define DEFINE_COLOR(color, opposite) case cName(color): return #color;
     #include "color.h"
     #undef DEFINE_COLOR
     default: return "<unknown>";
    };
}

Color StringToColor(string s) {
    #define DEFINE_COLOR(color, opposite) if (ColorToString(cName(color)) == s) { return cName(color);}
    #include "color.h"
    #undef DEFINE_COLOR
    return cName(NOT_A_COLOR);
}

