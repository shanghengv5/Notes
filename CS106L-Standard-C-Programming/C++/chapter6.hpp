//
//  chapter6.hpp
//  C++
//
//  Created by qt on 2024/3/21.
//

#ifndef chapter6_hpp
#define chapter6_hpp
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include "helper.hpp"

using namespace std;

struct DFA {
    map<pair<int, char>, int> transistions;
    set<int> acceptingStates;
    int startState;
};

struct NFA {
    multimap<pair<int, char>, int> transitions;
    set<int> acceptingStates;
    int startState;
};

int SixSidedDie();
long RepeatNum();
void PrintSteps();
set<string> LoadKeywords();
void PreprocessString(string& text);
void PrintReserved();
map<string, size_t> GenerateKeywordReport(string fileCotents);

bool SimulateDFA(DFA& d, string input);
bool SimulateNFA(NFA& n, string input);
bool IsEmailAddress(string input);
DFA LoadEmailDFA();
#endif /* chapter6_hpp */
