/*
  ==============================================================================

    GlobalControls.cpp
    Created: 17 Aug 2023 8:57:12am
    Author:  kylew

  ==============================================================================
*/

#include "GlobalControls.h"

GlobalControls::GlobalControls(SimpleSynthAudioProcessor& ap) :
    bypass1AT(ap.apvts, "bypassSynth1", bypass1), bypass2AT(ap.apvts, "bypassSynth2", bypass2), 
    bypassFilterAT(ap.apvts, "bypassFilter", bypassFilter), gainAT(ap.apvts, "gGain", globalGain),
    oscilloscope(ap), osc2(ap), fft(ap.fftData)
{
    setLookAndFeel(&lnf);
    addAndMakeVisible(outMeter[0]);
    addAndMakeVisible(outMeter[1]);
    addAndMakeVisible(oscilloscope);
    addAndMakeVisible(osc2);
    addAndMakeVisible(fft);

    setRotarySlider(globalGain);

    addAndMakeVisible(bypass1);
    bypass1.setComponentID("Power");
    addAndMakeVisible(bypass2);
    bypass2.setComponentID("Power");
    addAndMakeVisible(bypassFilter);
    bypassFilter.setComponentID("Power");

}

GlobalControls::~GlobalControls()
{
    setLookAndFeel(nullptr);
}

void GlobalControls::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    auto logoArea = bounds.removeFromLeft(bounds.getWidth() * .2).reduced(5);

    auto simpleArea = logoArea.removeFromLeft(logoArea.getWidth() * .4);
    auto maskArea = logoArea.removeFromLeft(logoArea.getWidth() * .33);
    auto synthArea = logoArea.removeFromLeft(logoArea.getWidth());

    auto logo = juce::ImageCache::getFromMemory(BinaryData::KITIK_LOGO_NO_BKGD_png, BinaryData::KITIK_LOGO_NO_BKGD_pngSize);
    //g.drawImage(logo, maskArea.toFloat());
    g.drawImage(logo, maskArea.toFloat(), juce::RectanglePlacement::fillDestination);

    auto newFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::OFFSHORE_TTF, BinaryData::OFFSHORE_TTFSize));
    g.setColour(juce::Colours::whitesmoke);
    g.setFont(newFont);
    g.setFont(30.f);
    g.drawFittedText("Simple", simpleArea, juce::Justification::centredRight, 1);
    g.drawFittedText("Synth", synthArea, juce::Justification::centredLeft, 1);

    auto synth1Area = bounds.removeFromLeft(bounds.getWidth() * .17);
    g.setFont(juce::Font::Font());
    g.drawFittedText(bypass1.getName(), synth1Area, juce::Justification::right, 1);

    auto synth2Area = bounds.removeFromLeft(bounds.getWidth() * .21);
    g.drawFittedText(bypass2.getName(), synth2Area, juce::Justification::right, 1);

    auto filterArea = bounds.removeFromRight(bounds.getWidth() * .45);
    g.drawFittedText(bypassFilter.getName(), filterArea, juce::Justification::left, 1);

    g.setColour(juce::Colours::white);

    auto fftArea = bounds.removeFromRight(bounds.getWidth() * .5).reduced(5);
    auto oscArea = bounds.reduced(5);
    g.fillRect(fftArea);
    g.fillRect(oscArea);

    g.setColour(juce::Colours::black);
}

void GlobalControls::resized()
{
    auto bounds = getLocalBounds();
    auto logoArea = bounds.removeFromLeft(bounds.getWidth() * .2).reduced(5);
    auto bp1Area = bounds.removeFromLeft(bounds.getWidth() * .05);
    auto bp2Area = bounds.removeFromLeft(bounds.getWidth() * .32);

    bypass1.setBounds(bp1Area);
    bypass2.setBounds(bp2Area);

    auto meterRight = bounds.removeFromRight(bounds.getWidth() * .045);
    auto meterLeft = bounds.removeFromRight(bounds.getWidth() * .05);
    auto gGainArea = bounds.removeFromRight(bounds.getWidth() * .15);
    auto filterArea = bounds.removeFromRight(bounds.getWidth() * .1);

    outMeter[0].setBounds(meterLeft);
    outMeter[1].setBounds(meterRight);
    globalGain.setBounds(gGainArea);
    bypassFilter.setBounds(filterArea);

    auto fftArea = bounds.removeFromRight(bounds.getWidth() * .6).reduced(5);
    auto oscArea = bounds.reduced(5);
    oscArea.translate(-8, 0);
    oscilloscope.setBounds(oscArea);

    fftArea.translate(-7, 0);
    fftArea.removeFromRight(fftArea.getWidth() * .35);
    fft.setBounds(fftArea);
}

void GlobalControls::setRotarySlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    slider.setComponentID("gGain");
    addAndMakeVisible(slider);
}

void GlobalControls::update(const std::vector<float> &values)
{
    enum
    {
        left,
        right
    };

    outMeter[0].setLevel(values[left]);
    outMeter[1].setLevel(values[right]);

    outMeter[0].repaint();
    outMeter[1].repaint();

    oscilloscope.repaint();
    osc2.repaint();
    fft.repaint();

    repaint();
}
