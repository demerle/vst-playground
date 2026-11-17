//
// Created by deeroot on 11/17/26.
//

#include "Square.h"


void Square::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::orange);
    g.setColour(juce::Colours::white);

    g.setFont(15.0f);

    g.drawFittedText("VST Playground", getLocalBounds(), juce::Justification::centred, 100);

}

void Square::resized()
{
    //titleLabel.setBounds (getLocalBounds().removeFromTop (40));
}