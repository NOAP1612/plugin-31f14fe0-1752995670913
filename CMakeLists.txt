CMakeLists.txt

cmake_minimum_required(VERSION 3.15)

project(StereoReverb VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# JUCE setup
find_package(JUCE CONFIG REQUIRED)

juce_add_plugin(StereoReverb
    COMPANY_NAME "YourCompany"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_ENABLED TRUE
    VST3 TRUE
    AU TRUE
    AUv3 TRUE
    VST2 FALSE
    LV2 FALSE
    AAX FALSE
    UNITY FALSE
    PRODUCT_NAME "StereoReverb"
)

target_sources(StereoReverb PRIVATE
    PluginProcessor.cpp
    PluginEditor.cpp
    PluginProcessor.h
    PluginEditor.h
)

target_link_libraries(StereoReverb PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)
```