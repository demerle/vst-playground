//
// Created by deeroot on 11/9/26.
//

#include "SineWave.h"

#include <cmath>

void SineWave::prepare(double sampleRate, int numChannels)
{
    currentSampleRate = static_cast<float>(sampleRate);
    timeIncrement = 1.0f / currentSampleRate;
    currentTime.resize(numChannels, 0.0f);
}

// y(t) = A * sin(2pi*f*t + phi)
//A = amplitude -> How large or small to make sine wave. In this case, between 0-1
// sin = sine wave function. Measures length of line as it travels around a circle
// 2pi = Complete circle in radians
// f = frequency of the wave (440 (arbitrary))
// t = point in time
// phi = phase offset (Not needed here but is extra/cool. You can start wave at different spots. We will skip this)

//output[s] = A * sin(spi * f * t)
// Arbitrary amplitude A = 0.5
//timeIncrement = 1/samplerate



void SineWave::process(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {

        auto* output = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            output[sample] = amplitude * std::sinf(juce::MathConstants<float>::twoPi * frequency * currentTime[channel]);
            currentTime[channel] = currentTime[channel] + timeIncrement;
        }
    }
}