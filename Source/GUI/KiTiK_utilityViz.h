/*
  ==============================================================================

    KiTiK_utilityViz.h
    Created: 4 Jan 2024 4:26:39pm
    Author:  kylew

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"

namespace kitik {

    struct FFTData
    {
        FFTData();
        ~FFTData();
        void pushNextSampleIntoFifo(juce::AudioBuffer<float> buffer);
        friend class FFT;

    protected:

        enum
        {
            fftOrder = 12,
            fftSize = 1 << fftOrder,
            scopeSize = 512
        };

        juce::dsp::FFT forwardFFT{ fftOrder };
        juce::dsp::WindowingFunction<float> window{ fftSize, juce::dsp::WindowingFunction<float>::hann };
        float fftData[2 * fftSize];
        float scopeData[scopeSize];
        float fifo[fftSize];
        int fifoIndex = 0;
        bool nextFFTBlockReady = false;
    };

    struct FFT : public juce::Component
    {
        FFT(FFTData& d);
        ~FFT();

        void paint(juce::Graphics& g) override;
        void resized() override;
        void drawNextFrame(FFTData& data);

    private:
        FFTData& data;
    };

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

}
