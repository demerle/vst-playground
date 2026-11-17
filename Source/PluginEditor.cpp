#include "PluginEditor.h"

AdditiveSynthAudioProcessorEditor::AdditiveSynthAudioProcessorEditor (AdditiveSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    titleLabel.setText ("Overtone Additive Synth", juce::dontSendNotification);
    titleLabel.setFont (juce::FontOptions (18.0f, juce::Font::bold));
    titleLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (titleLabel);

    frequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    frequencySlider.setRange(0.0f, 1.0f, 0.01f);
    addAndMakeVisible(frequencySlider);


    playButton.setButtonText("Play");
    playButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    playButton.setClickingTogglesState(true);
    playButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::red);
    playButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::green);
    addAndMakeVisible(playButton);

    frequencyLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(frequencyLabel);

    playButton.onClick = [this]()
    {
        //change state of the button when clicked
        const bool isPlaying = playButton.getToggleState();
        playButton.setButtonText(isPlaying ? "Playing" : "Play");
    };


    setSize (400, 300);
}

void AdditiveSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AdditiveSynthAudioProcessorEditor::resized()
{
    titleLabel.setBounds (getLocalBounds().removeFromTop (40));

    frequencySlider.setBounds(getWidth() / 2 - 150, getHeight() / 2 - 150, 300, 300);
    playButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 + 200, 100, 20);
    frequencyLabel.setBounds(getWidth() / 2 - 150, getHeight() / 2 - 150, 100, 20);

}

