//
//  chapter5.hpp
//  C++
//
//  Created by qt on 2024/3/13.
//

#ifndef chapter5_hpp
#define chapter5_hpp

#include <vector>
#include "helper.hpp"
const int kNumValues = 10;
using namespace std;

void SelectSort(vector<int>& values);
size_t GetSmallestIndex(vector<int>& values, size_t startIndex);
int GetInteger();
size_t InsertionIndex(vector<int>& values, int startIndex);
void PrintVectors();
#endif /* chapter5_hpp */
