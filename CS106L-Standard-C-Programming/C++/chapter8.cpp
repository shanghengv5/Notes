//
//  chapter8.cpp
//  C++
//
//  Created by qt on 2024/4/1.
//

#include "chapter8.hpp"

int FMRadio::getVolume() {
    return volume;
}
void FMRadio::setVolume(int newVol) {
    assert(newVol >= 0 && newVol <= 10);
    volume = newVol;
}

bool FMRadio::presetExists(int index) {
    assert(index >= 1 && index <= 6);
    return presets[index - 1] == 0.0; // -1 maps [1, 6] to [0, 5]
}

void FMRadio::setPreset(int index, double freq) {
    assert(index >= 1 && index <= 6);
    assert(freq >= 87.5 && freq <= 108.0);
    presets[index] = freq;
}

double FMRadio::getPreset(int index) {
    assert(presetExists(index));
    return presets[index];
}
