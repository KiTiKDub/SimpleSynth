/*
  ==============================================================================

    filter.h
    Created: 8 Aug 2023 5:43:01pm
    Author:  kylew

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "KiTiKLNF.h"

struct FilterComp : public juce::Component
{
    FilterComp(juce::AudioProcessorValueTreeState& apvts);
    ~FilterComp();

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setSlider(juce::Slider&);
    void setHorzSlider(juce::Slider&);
    void setRotarySlider(juce::Slider&);
    int getComboValue();

private:
    Laf lnf;

    juce::ComboBox filterType{ "Filter Type" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeAT;

    juce::ComboBox ladderMode  {"Ladder Type"};
    juce::Slider   ladderFreq  {"Frequency"}, 
                   ladderRes   {"Resonance"}, 
                   ladderDrive { "Drive" };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ladderModeAT;
    juce::AudioProcessorValueTreeState::SliderAttachment ladderFreqAT, ladderResAT, ladderDriveAT;

    juce::Slider   phaserRate       {"Rate"},
                   phaserDepth      {"Depth"},
                   phaserCenterFreq {"Frequency"},
                   phaserFeedback   {"Feedback"}, 
                   phaserMix        { "Mix" };
    juce::AudioProcessorValueTreeState::SliderAttachment phaserRateAT, phaserDepthAT, phaserCenterFreqAT, phaserFeedbackAT, phaserMixAT;

    juce::Slider   combFreq     { "Frequency" },
                   combFeedback { "Feedback" },
                   combGain     { "Gain" },
                   combMix      { "Mix" };
    juce::AudioProcessorValueTreeState::SliderAttachment combFreqAT, combFeeedbackAT, combGainAT, combMixAT;
};