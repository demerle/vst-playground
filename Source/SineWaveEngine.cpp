//
// Created by deeroot on 11/9/26.
//

#include "SineWaveEngine.h"

#include <cassert>
#include <cmath>

void SineWaveEngine::prepare(double sampleRate)
{
    currentSampleRate = sampleRate;
    phase = 0.0;
    smoothedFreq.reset(sampleRate, 0.05);
    gain.reset(sampleRate, 0.02);
    for (auto& amp : harmonicAmps)
        amp.reset(sampleRate, 0.02);
}

// y(t) = A * sin(2pi*f*t + phi)
//A = amplitude -> How large or small to make sine wave. In this case, between 0-1
// sin = sine wave function. Measures length of line as it travels around a circle
// 2pi = Complete circle in radians
// f = frequency of the wave (440 (arbitrary))
// t = point in time
// phi = phase offset (Not needed here but is extra/cool. You can start wave at different spots. We will skip this)

// Additive version: y(t) = sum over k of A_k * sin(k * 2pi*f*t)
// Every harmonic is a whole-number multiple of the fundamental, so one master
// phase drives them all and they stay locked together when the frequency glides.
// Harmonics above Nyquist (sampleRate / 2) are skipped to avoid aliasing.

void SineWaveEngine::process(juce::AudioBuffer<float>& buffer)
{
    assert (currentSampleRate > 0.0);
    constexpr double doublePi = juce::MathConstants<double>::twoPi;
    // Fixed headroom rather than normalising by the amplitude sum, so loudness
    // reflects the spectrum honestly. 16 harmonics all at 1.0 will still clip.
    constexpr float headroom = 0.25f;
    const double nyquist = currentSampleRate * 0.5;

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        const double frequency = smoothedFreq.getNextValue();

        //its just a min function with various type handling etc
        //This finds how many harmonics are able to play without going over nyquist / breaking the sound.
        const int maxHarmonic = juce::jmin(numHarmonics, static_cast<int>(nyquist / frequency));

        float value = 0.0f;
        for (int k = 1; k <= numHarmonics; ++k) {
            // Always advance every smoother so they stay in sync even when skipped
            const float amp = harmonicAmps[static_cast<size_t>(k)].getNextValue();
            if (k <= maxHarmonic && amp > 0.0f)
                value += amp * static_cast<float>(std::sin(phase * k));
        }

        value *= gain.getNextValue() * headroom;

        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            buffer.setSample(channel, sample, value);

        phase += (doublePi * frequency) / currentSampleRate;
        if (phase >= doublePi)
            phase -= doublePi;
    }
}
