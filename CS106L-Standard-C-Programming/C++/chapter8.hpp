//
//  chapter8.hpp
//  C++
//
//  Created by qt on 2024/4/1.
//

#ifndef chapter8_hpp
#define chapter8_hpp

#include <cassert>

class FMRadio {
public:
    double getFrequency();
    void setFrequency();
    
    int getVolume();
    void setVolume(int newVolume);
    void setPreset(int index, double freq);
    bool presetExists(int index);
    double getPreset(int index);
    private:
    double frequency;
    int volume;
    double presets[6];
};

#endif /* chapter8_hpp */
