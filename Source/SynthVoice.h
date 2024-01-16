/*
  ==============================================================================

    SynthVoice.h
    Created: 2 Aug 2023 10:44:33am
    Author:  kylew

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "DSP/oscData.h"

struct SynthVoice : public juce::SynthesiserVoice
{
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void update(float attack, float decay, float sustain, float release, float gain);
    oscData& getOscillator() { return osc; }
    float getPitchWheel();
    float getModWheel();
    void setOctave(int octave);
    //Visualizer viz;

private:
    oscData osc;
    oscData fmOsc;
    adsrData adsr;
    juce::AudioBuffer<float> synthBuffer;

    float customPitchWheelVal = 5;
    float customModWheelVal = 5;
    float pitchOctave = 0;
};

struct Visualizer : public juce::AudioVisualiserComponent
{
    Visualizer() : AudioVisualiserComponent(1)
    {
        setRepaintRate(60);
        setBufferSize(16);
        setSamplesPerBlock(256);
        setColours(juce::Colours::black, juce::Colours::white);
    }
};