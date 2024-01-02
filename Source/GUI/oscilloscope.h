/*
  ==============================================================================

    oscilloscope.h
    Created: 2 Jan 2024 3:12:58pm
    Author:  kylew

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "..//PluginProcessor.h"

struct OscilloscopeComp : public juce::Component
{
    OscilloscopeComp(SimpleSynthAudioProcessor& p);
    ~OscilloscopeComp();

    void paint(juce::Graphics&) override;
    void resized() override;

private:

    SimpleSynthAudioProcessor& audioProcessor;

    std::vector<float> audioPoints;
};