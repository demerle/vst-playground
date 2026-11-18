//
// Created by deeroot on 11/9/26.
//

#ifndef ADDITIVESYNTH_SINEWAVEENGINE_H
#define ADDITIVESYNTH_SINEWAVEENGINE_H

#include <juce_audio_basics/juce_audio_basics.h>
#include <array>

// Additive oscillator: sums sines at f, 2f, 3f... each with its own amplitude.
class SineWaveEngine {

public:
    static constexpr int numHarmonics = 16;

    void prepare(double sampleRate);
    void process(juce::AudioBuffer<float>& buffer);

    [[nodiscard]] float getFrequency() const { return smoothedFreq.getTargetValue(); }
    void setFrequency(const float newFrequency) { smoothedFreq.setTargetValue(newFrequency); }
    void setGain(const float newGain) { gain.setTargetValue(newGain); }

    // k is the harmonic number, 1..numHarmonics
    void setHarmonicAmp(const int k, const float amp) { harmonicAmps[static_cast<size_t>(k)].setTargetValue(amp); }

private:
    double currentSampleRate = 0.0;
    double phase = 0.0; // master phase; harmonic k = sin(k * phase)
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> smoothedFreq { 220.0f };
    juce::SmoothedValue<float> gain;
    std::array<juce::SmoothedValue<float>, numHarmonics + 1> harmonicAmps; // index 0 unused
};


#endif //ADDITIVESYNTH_SINEWAVEENGINE_H
