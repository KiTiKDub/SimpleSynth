/*
  ==============================================================================

    SynthVoice.cpp
    Created: 2 Aug 2023 10:44:33am
    Author:  kylew

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr; //Make sure we have an actual synth sound
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    float bendAmount = juce::jmap((float)currentPitchWheelPosition, 0.f, 16383.f, .89f, 1.12f);
    customPitchWheelVal = juce::jmap((float)currentPitchWheelPosition, 0.f, 16383.f, 0.f, 10.f);

    midiNoteNumber += pitchOctave * 12 + pitchSemi;

    osc.setWaveFreq(midiNoteNumber, false, bendAmount);
    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();
    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
    float bendAmount = juce::jmap((float)newPitchWheelValue, 0.f, 16383.f, .89f, 1.12f);
    customPitchWheelVal = juce::jmap((float)newPitchWheelValue, 0.f, 16383.f, 0.f, 10.f);

    osc.setWaveFreq(49, true, bendAmount);
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
    if (controllerNumber == 1)
    {
        customModWheelVal = juce::jmap((float)newControllerValue, 0.f, 127.f, 0.f, 10.f);
    }
}

void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
    if (!isVoiceActive())
        return;

    //Gets rid of clipping when switching between notes
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    auto block = juce::dsp::AudioBlock<float>(synthBuffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    osc.processNextBlock(context);

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    for (auto ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
    {
        outputBuffer.addFrom(ch, startSample, synthBuffer, ch, 0, numSamples);

        if (!adsr.isActive())
        {
            clearCurrentNote();
        }
            
    }
}

void SynthVoice::update(float attack, float decay, float sustain, float release, float gain)
{
    adsr.setADSR(attack, decay, sustain, release);
    osc.setGain(gain);
}

float SynthVoice::getPitchWheel()
{
    return customPitchWheelVal;
}

float SynthVoice::getModWheel()
{
    return customModWheelVal;
}

void SynthVoice::setOctave(int octave)
{
    pitchOctave = octave;
}

void SynthVoice::setSemi(int semitone)
{
    pitchSemi = semitone;
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = outputChannels;
    spec.sampleRate = sampleRate;

    osc.prepareToPlay(spec);
}
