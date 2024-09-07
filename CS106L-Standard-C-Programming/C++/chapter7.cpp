//
//  chapter7.cpp
//  C++
//
//  Created by qt on 2024/3/25.
//

#include "chapter7.hpp"

void PrintAverage() {
    ifstream input("/Users/qt/Documents/C++/C++/C++/data.txt");
    multiset<int> values;
    
    int currValue;
    while(input >> currValue)
        values.insert(currValue);
    
    double total = 0.0;
    for(multiset<int>::iterator itr = values.begin(); itr != values.end(); ++itr)
        total += *itr;
    
    cout << "Average is: " << total / values.size() << endl;
}

bool isNotAlpha(char ch) {
    return !isalpha(ch);
}

bool isNotWordOrWhiteSpace(char ch) {
    return isNotAlpha(ch) && !isspace(ch);
}

bool IsPalindrome(string input) {
    input.erase(remove_if(input.begin(), input.end(), isNotAlpha), input.end());
    transform(input.begin(), input.end(), input.begin(),  ::toupper);
    return equal(input.begin(), input.begin() + input.size() / 2, input.rbegin());
}

bool IsWordPalindrome(string input) {
    input.erase(remove_if(input.begin(), input.end(), isNotWordOrWhiteSpace), input.end());
    transform(input.begin(), input.end(), input.begin(),  ::toupper);
    
    stringstream tokenizer(input);
    vector<string> tokens;
    tokens.insert(tokens.begin(), istream_iterator<string>(tokenizer), istream_iterator<string>());
    return equal(tokens.begin(), tokens.begin() + tokens.size() / 2, tokens.rbegin());
}
