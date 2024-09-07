//
//  chapter6.cpp
//  C++
//
//  Created by qt on 2024/3/21.
//

#include "chapter6.hpp"

bool SimulateNFA(NFA& nfa, string input) {
    /* Track our set of states. We begin in the start state. */
    set<int> currStates;
    currStates.insert(nfa.startState);
    for(string::iterator itr = input.begin(); itr != input.end(); ++itr) {
        set<int> nextStates;
        for(set<int>::iterator state = currStates.begin();
            state != currStates.end(); ++state) {
            /* Get all states that we transition to from this current state. */
            pair<multimap<pair<int, char>, int>::iterator,
            multimap<pair<int, char>, int>::iterator>
            transitions = nfa.transitions.equal_range(make_pair(*state, *itr));
            
            /* Add these new states to the nextStates set. */
            for(; transitions.first != transitions.second; ++transitions.first)
            /* transitions.first is the current iterator, and its second
             * field is the value (new state) in the STL multimap.
             */
                nextStates.insert(transitions.first->second);
        }
        currStates = nextStates;
    }
    
    for(set<int>::iterator itr = currStates.begin(); itr != currStates.end(); itr++)
        if(nfa.acceptingStates.count(*itr)) return true;

    return false;
}

bool SimulateDFA(DFA& d, string input) {
    int currentState = d.startState;
    for(string::iterator itr = input.begin(); itr != input.end(); itr++) {
        currentState = d.transistions[make_pair(currentState, *itr)];
    }
    return d.acceptingStates.find(currentState) != d.acceptingStates.end();
}

bool IsEmailAddress(string input) {
    DFA emailChecker = LoadEmailDFA();
    
    for(string::iterator itr = input.begin(); itr != input.end(); ++itr) {
        if(isalnum(*itr))
            *itr = 'a';
        else if (*itr != '.' && *itr != '@')
            return false;
    }
    return SimulateDFA(emailChecker, input);
}

DFA LoadEmailDFA() {
    DFA d;
    d.startState = 0;
    d.acceptingStates.insert(6);
    // q0
    d.transistions.insert(make_pair(make_pair(0, 'a'), 1));
    d.transistions.insert(make_pair(make_pair(0, '.'), 7));
    d.transistions.insert(make_pair(make_pair(0, '@'), 7));
    // q1
    d.transistions.insert(make_pair(make_pair(1, 'a'), 1));
    d.transistions.insert(make_pair(make_pair(1, '.'), 2));
    d.transistions.insert(make_pair(make_pair(1, '@'), 3));
    // q2
    d.transistions.insert(make_pair(make_pair(2, 'a'), 1));
    d.transistions.insert(make_pair(make_pair(2, '.'), 7));
    d.transistions.insert(make_pair(make_pair(2, '@'), 7));
    // q3
    d.transistions.insert(make_pair(make_pair(3, 'a'), 4));
    d.transistions.insert(make_pair(make_pair(3, '.'), 7));
    d.transistions.insert(make_pair(make_pair(3, '@'), 7));
    // q4
    d.transistions.insert(make_pair(make_pair(4, 'a'), 4));
    d.transistions.insert(make_pair(make_pair(4, '.'), 5));
    d.transistions.insert(make_pair(make_pair(4, '@'), 3));
    // q5
    d.transistions.insert(make_pair(make_pair(5, 'a'), 6));
    d.transistions.insert(make_pair(make_pair(5, '.'), 7));
    d.transistions.insert(make_pair(make_pair(5, '@'), 7));
    // q6
    d.transistions.insert(make_pair(make_pair(6, 'a'), 6));
    d.transistions.insert(make_pair(make_pair(6, '.'), 5));
    d.transistions.insert(make_pair(make_pair(6, '@'), 7));

    return d;
}

int SixSidedDie() {
    return (rand() % 5) + 1;
}

set<string> LoadKeywords() {
    ifstream input("/Users/qt/Documents/C++/C++/C++/keywords.txt");
    set<string> result;
    string keyword;
    while(input >> keyword) {
        result.insert(keyword);
    }

//    for(set<string>::iterator itr = result.begin();itr != result.end(); ++itr)
//        cout << "Keywords: " << *itr << endl;
    return result;
}

void PrintReserved() {
    ifstream input;
    OpenUserFile(input);
    map<string, size_t> report = GenerateKeywordReport(GetFileContents(input));
    
    for(map<string, size_t>::iterator itr = report.begin();
        itr != report.end(); ++itr)
        cout << "Keyword " << itr->first << " occurred " << itr->second << " times." << endl;
        
}

map<string, size_t> GenerateKeywordReport(string fileCotents) {
    set<string> keywords = LoadKeywords();
    PreprocessString(fileCotents);
    
    stringstream tokenizer;
    tokenizer << fileCotents;
    
    string word;
    map<string, size_t> result;
    while(tokenizer >> word) {
        if (keywords.count(word))
            ++result[word];
    }
    
    return result;
}

long RepeatNum() {
    multiset<int> rolls;
    while(true) {
        int nextRoll = SixSidedDie();
        if(rolls.count(nextRoll) == 3) return rolls.size()+1;
        rolls.insert(nextRoll);
    }
}

void PrintSteps() {
    srand(static_cast<unsigned>(time(NULL)));
    size_t total = 0;
    for(size_t k = 0; k < 10000; ++k) {
        total += RepeatNum();
    }
    
    cout << "Average number of steps:" << double(total) / 10000 << endl;
}
