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
    voicesAT(ap.apvts, "voices", voices),
    oscilloscope(ap.oscData), fft(ap.fftData)
{
    setLookAndFeel(&lnf);
    addAndMakeVisible(outMeter[0]);
    addAndMakeVisible(outMeter[1]);
    addAndMakeVisible(oscilloscope);
    addAndMakeVisible(fft);

    setLinearSlider(globalGain);
    setRotarySlider(voices);

    addAndMakeVisible(bypass1);
    bypass1.setComponentID("Power");
    addAndMakeVisible(bypass2);
    bypass2.setComponentID("Power");
    addAndMakeVisible(bypassFilter);
    bypassFilter.setComponentID("Power");

    openPresetManager.setToggleState(false, juce::sendNotification);
    addAndMakeVisible(openPresetManager);

    openPresetManager.setClickingTogglesState(true);

    openPresetManager.onClick = [this]()
        {
            isShowTrue(openPresetManager.getToggleState()); //This is having the same glitch where its backwards and not working on first click
        };

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
    auto maskArea = logoArea.removeFromLeft(logoArea.getWidth() * .33).expanded(5,5);
    auto synthArea = logoArea.removeFromLeft(logoArea.getWidth());

    auto logo = juce::ImageCache::getFromMemory(BinaryData::KITIK_LOGO_NO_BKGD_png, BinaryData::KITIK_LOGO_NO_BKGD_pngSize);
    openPresetManager.setImages(true, true, true, logo, 0, juce::Colours::white, juce::Image(), 0, juce::Colours::white, juce::Image(), 0, juce::Colour(64u, 194u, 230u));
    openPresetManager.setBounds(maskArea);
    //g.drawImage(logo, maskArea.toFloat(), juce::RectanglePlacement::fillDestination);

    auto newFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::OFFSHORE_TTF, BinaryData::OFFSHORE_TTFSize));
    g.setColour(juce::Colours::whitesmoke);
    g.setFont(newFont);
    g.setFont(30.f);
    g.drawFittedText("Simple", simpleArea, juce::Justification::centredRight, 1);
    g.drawFittedText("Synth", synthArea, juce::Justification::centredLeft, 1);

    auto synth1Area = bounds.removeFromLeft(bounds.getWidth() * .17);
    g.setFont(juce::Font::Font());
    g.drawFittedText(bypass1.getName(), synth1Area, juce::Justification::centred, 1);

    auto synth2Area = bounds.removeFromLeft(bounds.getWidth() * .09);
    g.drawFittedText(bypass2.getName(), synth2Area, juce::Justification::centred, 1);

    auto filterArea = bounds.removeFromRight(bounds.getWidth() * .45);
    filterArea.removeFromRight(filterArea.getWidth() * .53);
    filterArea.removeFromLeft(filterArea.getWidth() * .6);
    filterArea.translate(-3, 0);
    g.drawFittedText(bypassFilter.getName(), filterArea, juce::Justification::right, 1);
    //g.drawRect(filterArea);

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
    auto bp2Area = bounds.removeFromLeft(bounds.getWidth() * .2);

    bypass1.setBounds(bp1Area);
    bypass2.setBounds(bp2Area);

    auto meterRight = bounds.removeFromRight(bounds.getWidth() * .035);
    auto meterLeft = bounds.removeFromRight(bounds.getWidth() * .04);
    auto gGainArea = bounds.removeFromRight(bounds.getWidth() * .1);
    auto filterArea = bounds.removeFromRight(bounds.getWidth() * .1);

    outMeter[0].setBounds(meterLeft);
    outMeter[1].setBounds(meterRight);
    globalGain.setBounds(gGainArea);
    bypassFilter.setBounds(filterArea);

    auto fftArea = bounds.removeFromRight(bounds.getWidth() * .63).reduced(5);
    auto oscArea = bounds.reduced(7,5);
    oscArea.translate(1, 0);
    oscilloscope.setBounds(oscArea);

    fftArea.removeFromRight(fftArea.getWidth() * .43);
    fft.setBounds(fftArea);

    auto newBounds = getLocalBounds();
    newBounds.removeFromLeft(logoArea.getWidth() + bypass1.getWidth() + bypass2.getWidth() + oscArea.getWidth() + fftArea.getWidth());
    newBounds.removeFromRight(meterRight.getWidth() + meterLeft.getWidth() + gGainArea.getWidth() + filterArea.getWidth());
    newBounds.reduce(20, -10);
    voices.setBounds(newBounds);

}

void GlobalControls::setLinearSlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    slider.setComponentID("gGain");
    addAndMakeVisible(slider);
}

void GlobalControls::setRotarySlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    slider.setTooltip("Voices");
    slider.setComponentID("Voice");
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
    fft.repaint();

    repaint();
}

void GlobalControls::isShowTrue(bool toggleState)
{
    presetManagerView = toggleState;
}

bool GlobalControls::showPresetManager()
{
    return presetManagerView;
}
