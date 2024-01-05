/*
  ==============================================================================

    KiTiK_utilityViz.h
    Created: 4 Jan 2024 4:26:39pm
    Author:  kylew

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace kitik {

    struct FFTData
    {

        void pushNextSampleIntoFifo(juce::AudioBuffer<float> buffer)
        {
            auto* data = buffer.getReadPointer(0);

            for (int i = 0; i < buffer.getNumSamples(); i++)
            {
                if (fifoIndex == fftSize)
                {
                    if (!nextFFTBlockReady)
                    {
                        juce::zeromem(fftData, sizeof(fftData));
                        memcpy(fftData, fifo, sizeof(fifo));
                        nextFFTBlockReady = true;
                    }

                    fifoIndex = 0;
                }

                fifo[fifoIndex++] = data[i];
            }
        }

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
        FFT(FFTData& d)
            : data(d)
        {

        }
        ~FFT()
        {

        }

        void paint(juce::Graphics& g) override
        {
            auto bounds = getLocalBounds();
            float width = bounds.getWidth();
            float height = bounds.getHeight();

            if (data.nextFFTBlockReady)
            {
                drawNextFrame(data);
                data.nextFFTBlockReady = false;
            }
            
            juce::Path p;

            p.startNewSubPath(0, height/2);

            for (int i = 1; i < data.scopeSize; ++i)
            {
                auto point = juce::jmap(data.scopeData[i], 0.f, 1.f, (height - 1), 0.f);
                
                p.lineTo(i, point);
            }

            g.strokePath(p, juce::PathStrokeType(1));
        }
        void resized() override
        {

        }
        void drawNextFrame(FFTData& data)
        {
            data.window.multiplyWithWindowingTable(data.fftData, data.fftSize);
            data.forwardFFT.performFrequencyOnlyForwardTransform(data.fftData);

            float min_dB = -72.f;
            float max_dB = 0;

            for (int i = 0; i < data.scopeSize; i++)
            {
                auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)data.scopeSize));
                auto fftDataIndex = juce::jlimit(0, data.fftSize / 2, (int)(skewedProportionX * (float)data.fftSize * 0.5f));
                auto level = juce::jmap(juce::jlimit(min_dB, max_dB, juce::Decibels::gainToDecibels(data.fftData[fftDataIndex])
                    - juce::Decibels::gainToDecibels((float)data.fftSize)),
                    min_dB, max_dB, 0.0f, 1.0f);

                data.scopeData[i] = level;
            }
        }

    private:
        FFTData& data;
    };

}
