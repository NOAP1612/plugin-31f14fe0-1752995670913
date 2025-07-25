#include "PluginProcessor.h"
#include "PluginEditor.h"

StereoReverbAudioProcessor::StereoReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                       ),
#endif
    parameters(*this, nullptr, juce::Identifier("StereoReverbParams"),
    {
        std::make_unique<juce::AudioParameterFloat>("roomSize", "Room Size", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>("decayTime", "Decay Time", 0.1f, 10.0f, 2.0f)
    })
{
    roomSizeParam = parameters.getRawParameterValue("roomSize");
    decayTimeParam = parameters.getRawParameterValue("decayTime");
}

StereoReverbAudioProcessor::~StereoReverbAudioProcessor()
{
}

const juce::String StereoReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoReverbAudioProcessor::acceptsMidi() const
{
   return false;
}

bool StereoReverbAudioProcessor::producesMidi() const
{
    return false;
}

bool StereoReverbAudioProcessor::isMidiEffect() const
{
    return false;
}

double StereoReverbAudioProcessor::getTailLengthSeconds() const
{
    return 10.0;
}

int StereoReverbAudioProcessor::getNumPrograms()
{
    return 1;
}

int StereoReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereoReverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StereoReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void StereoReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void StereoReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    reverb.reset();
    reverb.prepare(spec);
}

void StereoReverbAudioProcessor::releaseResources()
{
    reverb.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereoReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // Only stereo in/out supported
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo()
        || layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
  #endif
}
#endif

void StereoReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    // Update reverb parameters
    juce::Reverb::Parameters params;
    params.roomSize = *roomSizeParam;
    // Map decayTime [0.1, 10.0] to damping [0.0, 1.0] inversely
    // And to wetLevel/dryLevel to make long decays more prominent
    float decay = *decayTimeParam;
    params.damping = juce::jlimit(0.0f, 1.0f, 1.0f - (decay - 0.1f) / 10.0f); // longer decay = less damping
    params.wetLevel = 0.33f + 0.33f * (decay / 10.0f);
    params.dryLevel = 1.0f - params.wetLevel * 0.5f;
    params.width = 1.0f;
    params.freezeMode = 0.0f;
    reverb.setParameters(params);

    juce::dsp::AudioBlock<float> block(buffer);
    reverb.process(juce::dsp::ProcessContextReplacing<float>(block));
}

bool StereoReverbAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* StereoReverbAudioProcessor::createEditor()
{
    return new StereoReverbAudioProcessorEditor (*this);
}

void StereoReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, true);
    parameters.state.writeToStream(stream);
}

void StereoReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    if (tree.isValid())
        parameters.state = tree;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoReverbAudioProcessor();
}
```

```cpp