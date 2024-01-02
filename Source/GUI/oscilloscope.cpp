/*
  ==============================================================================

    oscilloscope.cpp
    Created: 2 Jan 2024 3:12:58pm
    Author:  kylew

  ==============================================================================
*/

#include "oscilloscope.h"
#include "../PluginProcessor.h"

OscilloscopeComp::OscilloscopeComp(SimpleSynthAudioProcessor& p)
    : audioProcessor(p)
{
    
}

OscilloscopeComp::~OscilloscopeComp()
{
}

void OscilloscopeComp::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    auto height = bounds.getHeight();
    float width = bounds.getWidth();

    g.setColour(juce::Colours::black);

    juce::Path path;

    path.startNewSubPath(0, height / 2);

    auto readPointer = audioProcessor.getOscSample(0);
    auto size = audioProcessor.getOscSize();

    auto ratio = size / width;

    for (int point = 0; point < size; point += ratio)
    {
        audioPoints.push_back(readPointer[point]);
    }

    for (int i = 0; i < size; i++)
    {

        auto point = juce::jmap(readPointer[i], -1.f, 1.f, (float)height, 0.f);
        path.lineTo(i, point);
    }

    g.strokePath(path, juce::PathStrokeType(1));
}

void OscilloscopeComp::resized()
{
}
