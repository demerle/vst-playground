#include "PluginProcessor.h"
#include "PluginEditor.h"

AdditiveSynthAudioProcessor::AdditiveSynthAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
        ), state(*this, nullptr, "parameters", createParameters())
{
    playSoundParam = state.getRawParameterValue("playSound");
    frequencyParam = state.getRawParameterValue("freqHz");
    for (int k = 1; k <= SineWaveEngine::numHarmonics; ++k)
        harmonicParams[static_cast<size_t>(k)] = state.getRawParameterValue(harmonicParamId(k));
}

void AdditiveSynthAudioProcessor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
    sineWaveEngine.prepare(sampleRate);
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

    // Gain is smoothed inside the engine, so toggling Play fades instead of clicking
    const bool playSound = playSoundParam->load() > 0.5f;
    sineWaveEngine.setGain(playSound ? 1.0f : 0.0f);

    sineWaveEngine.setFrequency(frequencyParam->load());

    for (int k = 1; k <= SineWaveEngine::numHarmonics; ++k)
        sineWaveEngine.setHarmonicAmp(k, harmonicParams[static_cast<size_t>(k)]->load());

    // The engine writes every sample of every channel, so the buffer doesn't need clearing first
    sineWaveEngine.process(buffer);
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

void AdditiveSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = state.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void AdditiveSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (state.state.getType()))
            state.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AdditiveSynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AdditiveSynthAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Skewed so more slider travel goes to low notes, where there's room for many harmonics
    juce::NormalisableRange<float> freqRange (20.0f, 2000.0f);
    freqRange.setSkewForCentre (220.0f);

    layout.add (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"freqHz", 1}, "Frequency", freqRange, 220.0f));
    layout.add (std::make_unique<juce::AudioParameterBool>(juce::ParameterID {"playSound", 1}, "Play Sound", false));

    for (int k = 1; k <= SineWaveEngine::numHarmonics; ++k)
        layout.add (std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID {harmonicParamId(k), 1},
            "Harmonic " + juce::String(k),
            0.0f, 1.0f, k == 1 ? 1.0f : 0.0f));

    return layout;
}