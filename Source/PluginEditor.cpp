#include "PluginEditor.h"

AdditiveSynthAudioProcessorEditor::AdditiveSynthAudioProcessorEditor (AdditiveSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), freqSliderAttachment(processorRef.getState(), "freqHz", frequencySlider),
    playButtonAttachment(processorRef.getState(), "playSound", playButton)
{
    titleLabel.setText ("Overtone Additive Synth", juce::dontSendNotification);
    titleLabel.setFont (juce::FontOptions (18.0f, juce::Font::bold));
    titleLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (titleLabel);

    frequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 25);
    frequencySlider.setTextValueSuffix(" Hz");
    addAndMakeVisible(frequencySlider);


    playButton.setButtonText("Play");
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

    for (int i = 0; i < numHarmonics; ++i)
    {
        const int k = i + 1;
        auto& slider = harmonicSliders[static_cast<size_t>(i)];
        slider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        addAndMakeVisible(slider);
        harmonicAttachments[static_cast<size_t>(i)] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processorRef.getState(), AdditiveSynthAudioProcessor::harmonicParamId(k), slider);

        auto& label = harmonicLabels[static_cast<size_t>(i)];
        label.setText(juce::String(k), juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);
    }

    // The classic waveforms, built only from sines
    sinePresetButton.onClick     = [this] { applyPreset ([] (int k) { return k == 1 ? 1.0f : 0.0f; }); };
    sawPresetButton.onClick      = [this] { applyPreset ([] (int k) { return 1.0f / static_cast<float>(k); }); };
    squarePresetButton.onClick   = [this] { applyPreset ([] (int k) { return k % 2 == 1 ? 1.0f / static_cast<float>(k) : 0.0f; }); };

    for (auto* button : { &sinePresetButton, &sawPresetButton, &squarePresetButton })
        addAndMakeVisible(button);

    setSize (800, 400);
}

void AdditiveSynthAudioProcessorEditor::applyPreset (const std::function<float (int)>& ampForHarmonic)
{
    for (int k = 1; k <= numHarmonics; ++k)
    {
        if (auto* param = processorRef.getState().getParameter(AdditiveSynthAudioProcessor::harmonicParamId(k)))
        {
            param->beginChangeGesture();
            param->setValueNotifyingHost(param->convertTo0to1(ampForHarmonic(k)));
            param->endChangeGesture();
        }
    }
}

void AdditiveSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AdditiveSynthAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);

    titleLabel.setBounds (area.removeFromTop (30));

    // Preset buttons along the bottom
    auto presetRow = area.removeFromBottom(30);
    area.removeFromBottom(10);
    const int presetWidth = presetRow.getWidth() / 3;
    for (auto* button : { &sinePresetButton, &sawPresetButton, &squarePresetButton })
        button->setBounds(presetRow.removeFromLeft(presetWidth).reduced(4, 0));

    // Frequency and Play on the left
    auto leftColumn = area.removeFromLeft(100);
    area.removeFromLeft(10);
    frequencyLabel.setBounds(leftColumn.removeFromTop(20));
    playButton.setBounds(leftColumn.removeFromBottom(30).reduced(5, 0));
    leftColumn.removeFromBottom(5);
    frequencySlider.setBounds(leftColumn);

    // One slider per harmonic, numbered underneath
    auto labelRow = area.removeFromBottom(20);
    const int harmonicWidth = area.getWidth() / numHarmonics;
    for (int i = 0; i < numHarmonics; ++i)
    {
        harmonicSliders[static_cast<size_t>(i)].setBounds(area.removeFromLeft(harmonicWidth).reduced(3, 0));
        harmonicLabels[static_cast<size_t>(i)].setBounds(labelRow.removeFromLeft(harmonicWidth));
    }
}
