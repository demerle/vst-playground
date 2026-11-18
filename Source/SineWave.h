//
// Created by deeroot on 11/9/26.
//

#ifndef ADDITIVESYNTH_SINEWAVE_H
#define ADDITIVESYNTH_SINEWAVE_H

#include <juce_audio_basics/juce_audio_basics.h>

class SineWave {

public:
    void prepare(double sampleRate, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    [[nodiscard]] float getAmplitude() const {return amplitude;}
    [[nodiscard]] float getFrequency() { return smoothedFreq.getNextValue(); }
    void setAmplitude(const float newAmplitude) {amplitude = newAmplitude;}
    void setFrequency(const float newFrequency) {smoothedFreq.setTargetValue(newFrequency);}

private:
    float amplitude = 1.0f;
    float currentSampleRate = 0.0f;
    std::vector<float> phases;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> smoothedFreq;
};


#endif //ADDITIVESYNTH_SINEWAVE_H
