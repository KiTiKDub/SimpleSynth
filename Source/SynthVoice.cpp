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
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
    auto block = juce::dsp::AudioBlock<float>(outputBuffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    osc.process(context);
    oscGain.process(context);

    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = outputChannels;
    spec.sampleRate = sampleRate;

    osc.prepare(spec);
    oscGain.prepare(spec);

    oscGain.setGainLinear(.3f);

    
}
