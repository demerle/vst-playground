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
    static constexpr int numHarmonics = SineWaveEngine::numHarmonics;

    // Sets every harmonic slider at once; ampForHarmonic(k) returns the amplitude for harmonic k
    void applyPreset (const std::function<float (int)>& ampForHarmonic);

    AdditiveSynthAudioProcessor& processorRef;
    juce::Label titleLabel;
    juce::Slider frequencySlider;
    juce::TextButton playButton;
    juce::Label frequencyLabel {"FrequencyLabel", "Frequency"};

    juce::AudioProcessorValueTreeState::SliderAttachment freqSliderAttachment;

    juce::AudioProcessorValueTreeState::ButtonAttachment playButtonAttachment;

    // Index 0 is harmonic 1
    std::array<juce::Slider, numHarmonics> harmonicSliders;
    std::array<juce::Label, numHarmonics> harmonicLabels;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, numHarmonics> harmonicAttachments;

    juce::TextButton sinePresetButton {"Sine"};
    juce::TextButton sawPresetButton {"Saw"};
    juce::TextButton squarePresetButton {"Square"};


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdditiveSynthAudioProcessorEditor)
};
