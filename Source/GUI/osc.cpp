/*
  ==============================================================================

    adsr.cpp
    Created: 2 Aug 2023 2:32:05pm
    Author:  kylew

  ==============================================================================
*/

#include "osc.h"

OscComp::OscComp(juce::AudioProcessorValueTreeState& apvts, Visualizer& viz) :
    attackAT1(apvts, "attack1", attack1), attackAT2(apvts, "attack2", attack2),
    decayAT1(apvts, "decay1", decay1), decayAT2(apvts, "decay2", decay2),
    sustainAT1(apvts, "sustain1", sustain1), sustainAT2(apvts, "sustain2", sustain2),
    releaseAT1(apvts, "release1", release1), releaseAT2(apvts, "release2", release2),
    gainAT1(apvts, "oscGain1", gain1), gainAT2(apvts, "oscGain2", gain2),
    sineAT1(apvts, "sine1", sine1), sineAT2(apvts, "sine2", sine2),
    sawAT1(apvts, "saw1", saw1), sawAT2(apvts, "saw2", saw2),
    squareAT1(apvts, "square1", square1), squareAT2(apvts, "square2", square2),
    triangleAT1(apvts, "triangle1", triangle1), triangleAT2(apvts, "triangle2", triangle2),
    fmOscAT(apvts, "fmOsc", fmOsc), fmDepthAT(apvts, "fmDepth", fmDepth),
    voicesAT(apvts, "voices", voices)
{
    //vis = &viz;
    setRotarySlider(voices);
    setLookAndFeel(&lnf);
    setSlider(attack1);
    setSlider(decay1);
    setSlider(sustain1);
    setSlider(release1);
    setHorzSlider(gain1);

    addAndMakeVisible(sine1);
    addAndMakeVisible(saw1);
    addAndMakeVisible(square1);
    addAndMakeVisible(triangle1);

    sine1.onClick = [this] {updateToggleState(&sine1); };
    saw1.onClick = [this] {updateToggleState(&saw1); };
    square1.onClick = [this] {updateToggleState(&square1); };
    triangle1.onClick = [this] {updateToggleState(&triangle1); };

    sine1.setRadioGroupId(1);
    saw1.setRadioGroupId(1);
    square1.setRadioGroupId(1);
    triangle1.setRadioGroupId(1);

    setSlider(attack2);
    setSlider(decay2);
    setSlider(sustain2);
    setSlider(release2);
    setHorzSlider(gain2);

    addAndMakeVisible(sine2);
    addAndMakeVisible(saw2);
    addAndMakeVisible(square2);
    addAndMakeVisible(triangle2);

    sine2.onClick = [this] {updateToggleState(&sine2); };
    saw2.onClick = [this] {updateToggleState(&saw2); };
    square2.onClick = [this] {updateToggleState(&square2); };
    triangle2.onClick = [this] {updateToggleState(&triangle2); };

    sine2.setRadioGroupId(2);
    saw2.setRadioGroupId(2);
    square2.setRadioGroupId(2);
    triangle2.setRadioGroupId(2);

    fmOsc.setButtonText("FM");
    addAndMakeVisible(fmOsc);
    setRotarySlider(fmDepth);
    fmDepth.setTitle("fmDepth");

    addAndMakeVisible(oscWaves);
    addAndMakeVisible(oscWavesR);
}

OscComp::~OscComp()
{
    setLookAndFeel(nullptr);
}

void OscComp::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    auto bounds = getLocalBounds().reduced(5);
    auto boundsLeft = bounds.removeFromLeft(bounds.getWidth() * .5);
    auto boundsRight = bounds;
    //g.drawRect(boundsRight);

    auto oscPicture = boundsLeft.removeFromTop(boundsLeft.getHeight() * .5);
    auto adsrBounds = bounds.removeFromBottom(bounds.getHeight() * .4);
    auto waveTypeBounds = bounds.removeFromBottom(bounds.getHeight() * .166);
    auto gainBounds = bounds.removeFromBottom(bounds.getHeight() * .2);

    g.setColour(juce::Colours::black);

    g.fillRect(oscPicture);
    g.setColour(juce::Colours::white);
    g.drawRect(oscPicture);
    //g.drawFittedText("Soon...", oscPicture, juce::Justification::centred, 1);

    g.setColour(juce::Colours::black);

    auto oscPictureR = boundsRight.removeFromTop(boundsRight.getHeight() * .5).reduced(5);
    oscPictureR = oscPictureR.removeFromRight(oscPictureR.getWidth() * .8);
    auto adsrBounds2 = bounds.removeFromBottom(bounds.getHeight() * .4);
    auto waveTypeBounds2 = bounds.removeFromBottom(bounds.getHeight() * .166);
    auto gainBounds2 = bounds.removeFromBottom(bounds.getHeight() * .2);

    g.fillRect(oscPictureR);
    g.setColour(juce::Colours::white);
    g.drawRect(oscPictureR);
    //g.drawFittedText("Soon...", oscPictureR, juce::Justification::centred, 1);

}

void OscComp::resized()
{
    auto bounds = getLocalBounds();
    auto boundsLeft = bounds.removeFromLeft(bounds.getWidth() * .5).reduced(5);
    auto boundsRight = bounds.reduced(5);

    auto adsrBounds1 = boundsLeft.removeFromBottom(boundsLeft.getHeight() * .3);
    auto waveTypeBounds1 = boundsLeft.removeFromBottom(boundsLeft.getHeight() * .166);
    auto gainBounds1 = boundsLeft.removeFromBottom(boundsLeft.getHeight() * .18);
    gain1.setBounds(gainBounds1);

    auto sineBounds1 = waveTypeBounds1.removeFromLeft(waveTypeBounds1.getWidth() * .25);
    sine1.setBounds(sineBounds1);
    auto sawBounds1 = waveTypeBounds1.removeFromLeft(waveTypeBounds1.getWidth() * .33);
    saw1.setBounds(sawBounds1);
    auto squareBounds1 = waveTypeBounds1.removeFromLeft(waveTypeBounds1.getWidth() * .5);
    square1.setBounds(squareBounds1);
    auto triangleBounds1 = waveTypeBounds1.removeFromLeft(waveTypeBounds1.getWidth());
    triangle1.setBounds(triangleBounds1);

    auto attackBounds1 = adsrBounds1.removeFromLeft(adsrBounds1.getWidth() * .25);
    attack1.setBounds(attackBounds1);
    auto decayBounds1 = adsrBounds1.removeFromLeft(adsrBounds1.getWidth() * .33);
    decay1.setBounds(decayBounds1);
    auto sustainBounds1 = adsrBounds1.removeFromLeft(adsrBounds1.getWidth() * .5);
    sustain1.setBounds(sustainBounds1);
    auto releaseBounds1 = adsrBounds1.removeFromLeft(adsrBounds1.getWidth());
    release1.setBounds(releaseBounds1);

    boundsLeft.translate(5, 0);
    oscWaves.setBounds(boundsLeft);

    auto adsrBounds2 = boundsRight.removeFromBottom(boundsRight.getHeight() * .3);
    auto waveTypeBounds2 = boundsRight.removeFromBottom(boundsRight.getHeight() * .166);
    auto gainBounds2 = boundsRight.removeFromBottom(boundsRight.getHeight() * .18);
    gain2.setBounds(gainBounds2);

    auto fmBounds = boundsRight.removeFromLeft(boundsRight.getWidth() * .2);
    auto fmOscBounds = fmBounds.removeFromTop(fmBounds.getHeight() * .33);
    auto voiceBounds = fmBounds.removeFromTop(fmBounds.getHeight() * .5);
    voices.setBounds(voiceBounds);
    fmOsc.setBounds(fmOscBounds);
    fmDepth.setBounds(fmBounds);

    auto sineBounds2 = waveTypeBounds2.removeFromLeft(waveTypeBounds2.getWidth() * .25);
    sine2.setBounds(sineBounds2);
    auto sawBounds2 = waveTypeBounds2.removeFromLeft(waveTypeBounds2.getWidth() * .33);
    saw2.setBounds(sawBounds2);
    auto squareBounds2 = waveTypeBounds2.removeFromLeft(waveTypeBounds2.getWidth() * .5);
    square2.setBounds(squareBounds2);
    auto triangleBounds2 = waveTypeBounds2.removeFromLeft(waveTypeBounds2.getWidth());
    triangle2.setBounds(triangleBounds2);

    auto attackBounds2 = adsrBounds2.removeFromLeft(adsrBounds2.getWidth() * .25);
    attack2.setBounds(attackBounds2);
    auto decayBounds2 = adsrBounds2.removeFromLeft(adsrBounds2.getWidth() * .33);
    decay2.setBounds(decayBounds2);
    auto sustainBounds2 = adsrBounds2.removeFromLeft(adsrBounds2.getWidth() * .5);
    sustain2.setBounds(sustainBounds2);
    auto releaseBounds2 = adsrBounds2.removeFromLeft(adsrBounds2.getWidth());
    release2.setBounds(releaseBounds2);

    boundsRight = boundsRight.reduced(5);
    boundsRight.removeFromRight(boundsRight.getWidth() * .02);
    oscWavesR.setBounds(boundsRight);

}

void OscComp::setSlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    slider.setTooltip("Sec");
    addAndMakeVisible(slider);
}

void OscComp::setHorzSlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    slider.setTooltip("dB");
    addAndMakeVisible(slider);
}

void OscComp::setRotarySlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    slider.setComponentID("Filter");
    addAndMakeVisible(slider);
}

void OscComp::updateToggleState(juce::Button* button)
{
    auto state = button->getToggleState();
    if (button->getRadioGroupId() == 1)
    {
        oscWaves.setOscType(button->getButtonText());
    }
    else
    {
        oscWavesR.setOscType(button->getButtonText());
    }
    repaint();
}

OscWaves::OscWaves()
{
}

OscWaves::~OscWaves()
{
}

void OscWaves::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    auto height = bounds.getHeight();
    auto width = bounds.getWidth();
    auto centerX = bounds.getCentreX();

    juce::Path p;
    g.setColour(juce::Colours::white);

    if (!isSine)
    {
        p.startNewSubPath(bounds.getX(), height / 2);

        for (int i = 0; i < width; i+=5)
        {
            float yAxis = std::sin(2*3.14*i / width);
            auto point = juce::jmap(yAxis, -1.f, 1.f, (float)(height * .8), (float)(height * .2));
            p.lineTo(i, point);
        }

        g.strokePath(p, juce::PathStrokeType(1));
    }
    else if (!isSaw)
    {
        p.startNewSubPath(bounds.getX(), height / 2);
        p.lineTo(centerX, height / 5);
        p.lineTo(centerX, height * .8);
        p.lineTo(width, height / 2);

        g.strokePath(p, juce::PathStrokeType(1));
    }
    else if (!isSquare)
    {
        p.startNewSubPath(bounds.getX(), height / 5);
        p.lineTo(centerX, height / 5);
        p.lineTo(centerX, height * .8);
        p.lineTo(width, height * .8);

        g.strokePath(p, juce::PathStrokeType(1));
    }
    else
    {
        p.startNewSubPath(bounds.getX(), height / 2);
        p.lineTo(centerX - width / 4, height / 5);
        p.lineTo(centerX + width / 4, height * .8);
        p.lineTo(width, height / 2);

        g.strokePath(p, juce::PathStrokeType(1));
    }
}

void OscWaves::resized()
{
}

void OscWaves::setOscType(juce::String oscType)
{
    isSine = oscType.compare("Sine");
    isSaw = oscType.compare("Saw");
    isSquare = oscType.compare("Square");
    isTri = oscType.compare("Triangle");
    
}
