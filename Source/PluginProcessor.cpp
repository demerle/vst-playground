#include "PluginProcessor.h"
#include "PluginEditor.h"

AdditiveSynthAudioProcessor::AdditiveSynthAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
        ), state(*this, nullptr, "parameters", createParameters())
{
}

void AdditiveSynthAudioProcessor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
    sineWave.prepare(sampleRate, getTotalNumOutputChannels());
    playSoundParam = state.getRawParameterValue("playSound");
    frequencyParam = state.getRawParameterValue("freqHz");
}

void AdditiveSynthAudioProcessor::releaseResources() {}

bool AdditiveSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void AdditiveSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    midiMessages.clear();

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        juce::FloatVectorOperations::fill (buffer.getWritePointer (channel), 0.0f, buffer.getNumSamples());

    // Buffer was zeroed above, so returning early outputs silence
    const bool playSound = static_cast<bool>(playSoundParam->load());
    if (playSound)
        sineWave.setAmplitude(0.4f);
    else {
        sineWave.setAmplitude(0.0f);
    }

    const float freq = frequencyParam->load();
    sineWave.setFrequency(freq);

    sineWave.process(buffer);
}

juce::AudioProcessorEditor* AdditiveSynthAudioProcessor::createEditor()
{
    return new AdditiveSynthAudioProcessorEditor (*this);
}

bool AdditiveSynthAudioProcessor::hasEditor() const { return true; }

const juce::String AdditiveSynthAudioProcessor::getName() const { return JucePlugin_Name; }
bool AdditiveSynthAudioProcessor::acceptsMidi() const  { return true; }
bool AdditiveSynthAudioProcessor::producesMidi() const { return false; }
bool AdditiveSynthAudioProcessor::isMidiEffect() const { return false; }
double AdditiveSynthAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int AdditiveSynthAudioProcessor::getNumPrograms() { return 1; }
int AdditiveSynthAudioProcessor::getCurrentProgram() { return 0; }
void AdditiveSynthAudioProcessor::setCurrentProgram (int) {}
const juce::String AdditiveSynthAudioProcessor::getProgramName (int) { return {}; }
void AdditiveSynthAudioProcessor::changeProgramName (int, const juce::String&) {}

void AdditiveSynthAudioProcessor::getStateInformation (juce::MemoryBlock&) {}
void AdditiveSynthAudioProcessor::setStateInformation (const void*, int) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AdditiveSynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AdditiveSynthAudioProcessor::createParameters()
{
    return {
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"freqHz"}, "Frequency", 20.0f, 20000.0f, 220.0f),
        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"playSound"}, "Play Sound", false)
    };
}