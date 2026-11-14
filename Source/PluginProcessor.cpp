#include "PluginProcessor.h"
#include "PluginEditor.h"

AdditiveSynthAudioProcessor::AdditiveSynthAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
}

void AdditiveSynthAudioProcessor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
    sineWave.prepare(sampleRate, getTotalNumOutputChannels());
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
