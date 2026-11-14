#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "PluginProcessor.h"

class AdditiveSynthAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AdditiveSynthAudioProcessorEditor (AdditiveSynthAudioProcessor&);
    ~AdditiveSynthAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AdditiveSynthAudioProcessor& processorRef;
    juce::Label titleLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdditiveSynthAudioProcessorEditor)
};
