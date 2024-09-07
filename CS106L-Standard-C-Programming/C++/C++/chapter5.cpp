//
//  chapter5.cpp
//  C++
//
//  Created by qt on 2024/3/13.
//

#include "chapter5.hpp"

void  PrintVectors() {
    vector<int> values;
    for (size_t i = 0; i < kNumValues; i++) {
        cout << "Enter another value: ";
        int val = GetInteger();
        values.insert(values.begin() + InsertionIndex(values, val), val);
    }
    
    for(size_t i = 0; i < kNumValues; ++i) {
        cout << values[i] << endl;
    }
}



size_t InsertionIndex(vector<int>& values, int val) {
    for(size_t i = 0; i < values.size(); ++i)
        if (val < values[i])
            return i;
    return values.size();
}

void SelectSort(vector<int>& values) {
    for(size_t i = 0; i < values.size(); ++i) {
        size_t smallestIndex = GetSmallestIndex(values, i);
        swap(values[i], values[smallestIndex]);
    }
}

size_t GetSmallestIndex(vector<int>& values, size_t startIndex) {
    size_t bestIndex = startIndex;
    for(size_t i = startIndex; i < values.size(); ++i)
        if (values[i] <= values[startIndex])
            bestIndex = i;
    return bestIndex;
}
