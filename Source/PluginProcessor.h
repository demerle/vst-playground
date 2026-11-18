#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "SineWaveEngine.h"

class AdditiveSynthAudioProcessor : public juce::AudioProcessor
{
public:
    AdditiveSynthAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getState() { return state; }
    static juce::String harmonicParamId(int k) { return "h" + juce::String(k); }

private:
    //double currentSampleRate = 0.0;
    SineWaveEngine sineWaveEngine;
    juce::AudioProcessorValueTreeState state;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    std::atomic<float>* frequencyParam = nullptr;
    std::atomic<float>* playSoundParam = nullptr;
    std::array<std::atomic<float>*, SineWaveEngine::numHarmonics + 1> harmonicParams {}; // index 0 unused

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdditiveSynthAudioProcessor)
};
