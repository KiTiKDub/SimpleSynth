/*
  ==============================================================================

    GlobalControls.h
    Created: 17 Aug 2023 8:57:12am
    Author:  kylew

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "KiTiKLNF.h"
#include "KiTiK_utilityViz.h"

struct GlobalControls : public juce::Component
{
    GlobalControls(SimpleSynthAudioProcessor& ap);
    ~GlobalControls();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setRotarySlider(juce::Slider&);
    void update(const std::vector<float>& values);

    void isShowTrue(bool toggleState);
    bool showPresetManager();

private:

    Laf lnf;
    std::array<Laf::LevelMeter, 2> outMeter;

    OscilloscopeComp oscilloscope;
    FFTComp fft;

    juce::ToggleButton bypass1      { "Bypass Synth 1" },
                       bypass2      { "Bypass Synth 2" },
                       bypassFilter { "Bypass Filter" };

    juce::AudioProcessorValueTreeState::ButtonAttachment bypass1AT, bypass2AT, bypassFilterAT;

    juce::ImageButton openPresetManager;
    bool presetManagerView;

    juce::Slider globalGain;
    juce::AudioProcessorValueTreeState::SliderAttachment gainAT;

};