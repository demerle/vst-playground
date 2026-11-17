#include "PluginEditor.h"

AdditiveSynthAudioProcessorEditor::AdditiveSynthAudioProcessorEditor (AdditiveSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    titleLabel.setText ("4-Overtone Additive Synth", juce::dontSendNotification);
    titleLabel.setFont (juce::FontOptions (18.0f, juce::Font::bold));
    titleLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (titleLabel);

    addAndMakeVisible (square);

    setSize (400, 300);
}

void AdditiveSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AdditiveSynthAudioProcessorEditor::resized()
{
    titleLabel.setBounds (getLocalBounds().removeFromTop (40));

    square.setBounds(100,100, 200,200);
}

