//
//  x-macro.hpp
//  C++
//
//  Created by qt on 2024/3/11.
//

#ifndef x_macro_hpp
#define x_macro_hpp
#include <string>
using namespace std;

#define cName(color) eColor_##color
enum Color {
     #define DEFINE_COLOR(color, opposite) cName(color),
     #include "color.h"
     #undef DEFINE_COLOR
 };



Color GetOppositeColor(Color c);
string ColorToString(Color c);
Color StringToColor(Color c);
#endif /* x_macro_hpp */
