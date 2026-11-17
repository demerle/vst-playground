//
// Created by deeroot on 11/17/26.
//

#ifndef ADDITIVESYNTH_SQUARE_H
#define ADDITIVESYNTH_SQUARE_H

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


class Square : public juce::Component {

public:
    void paint (juce::Graphics&) override;
    void resized() override;

};


#endif //ADDITIVESYNTH_SQUARE_H
