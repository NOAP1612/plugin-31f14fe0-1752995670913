#include "PluginProcessor.h"
#include "PluginEditor.h"

StereoReverbAudioProcessorEditor::StereoReverbAudioProcessorEditor (StereoReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
      roomSizeAttachment(processor.parameters, "roomSize", roomSizeSlider),
      decayTimeAttachment(processor.parameters, "decayTime", decayTimeSlider)
{
    setSize (380, 180);

    // Room Size Slider
    roomSizeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    roomSizeSlider.setRange(0.0, 1.0, 0.01);
    roomSizeSlider.setSkewFactorFromMidPoint(0.5);
    roomSizeSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::deepskyblue);
    roomSizeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    addAndMakeVisible(roomSizeSlider);

    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeLabel.setJustificationType(juce::Justification::centred);
    roomSizeLabel.attachToComponent(&roomSizeSlider, false);
    addAndMakeVisible(roomSizeLabel);

    // Decay Time Slider
    decayTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    decayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    decayTimeSlider.setRange(0.1, 10.0, 0.01);
    decayTimeSlider.setSkewFactorFromMidPoint(1.0);
    decayTimeSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    decayTimeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    addAndMakeVisible(decayTimeSlider);

    decayTimeLabel.setText("Decay Time (s)", juce::dontSendNotification);
    decayTimeLabel.setJustificationType(juce::Justification::centred);
    decayTimeLabel.attachToComponent(&decayTimeSlider, false);
    addAndMakeVisible(decayTimeLabel);
}

StereoReverbAudioProcessorEditor::~StereoReverbAudioProcessorEditor()
{
}

void StereoReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(0xff181e26));
    g.setColour (juce::Colours::white);
    g.setFont (22.0f);
    g.drawFittedText ("Stereo Reverb", 0, 12, getWidth(), 28, juce::Justification::centred, 1);
}

void StereoReverbAudioProcessorEditor::resized()
{
    const int sliderWidth = 130;
    const int sliderHeight = 130;
    const int padding = 30;

    roomSizeSlider.setBounds(padding, 40, sliderWidth, sliderHeight);
    decayTimeSlider.setBounds(getWidth() - sliderWidth - padding, 40, sliderWidth, sliderHeight);
}
```

```cmake
#