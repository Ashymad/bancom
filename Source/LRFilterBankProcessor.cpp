/*
  ==============================================================================

    LRFilterBankProcessor.cpp
    Created: 6 Jan 2019 7:46:28pm
    Author:  shyman

  ==============================================================================
*/

#include "LRFilterBankProcessor.h"

LRFilterBankProcessor::LRFilterBankProcessor() :
    qValue(1),
    centerFrequency(1000),
    order(2),
    A1_L(IIRFilterCascade()),
    A1_H(IIRFilterCascade()),
    A2_L(IIRFilterCascade()),
    A2_H(IIRFilterCascade())
{
}

const String LRFilterBankProcessor::getName() const
{
    return "LRFilterBankProcessor";
}

void LRFilterBankProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    float lowerFrequency = centerFrequency*(1 - 1/(2*qValue));
    float upperFrequency = centerFrequency*(1 + 1/(2*qValue));

    A1_L.addFilterFromCoefficients(designIIRLowpassHighOrderLRMethod(upperFrequency, sampleRate, order));
    A1_H.addFilterFromCoefficients(designIIRHighpassHighOrderLRMethod(upperFrequency, sampleRate, order));
    A2_L.addFilterFromCoefficients(designIIRLowpassHighOrderLRMethod(lowerFrequency, sampleRate, order));
    A2_H.addFilterFromCoefficients(designIIRHighpassHighOrderLRMethod(lowerFrequency, sampleRate, order));
}

void LRFilterBankProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples();

    AudioSampleBuffer buffers[4];

    buffers[0] = AudioSampleBuffer(buffer);
    buffers[2] = AudioSampleBuffer(buffer);
    
    A1_L.processSamples(buffers[0].getWritePointer(0), numSamples);
    A1_H.processSamples(buffers[2].getWritePointer(0), numSamples);

    buffers[1] = AudioSampleBuffer(buffers[0]);
    buffers[3] = AudioSampleBuffer(buffers[2]);

    A2_L.processSamples(buffers[0].getWritePointer(0), numSamples);
    A2_H.processSamples(buffers[1].getWritePointer(0), numSamples);
    A2_L.processSamples(buffers[2].getWritePointer(0), numSamples);
    A2_H.processSamples(buffers[3].getWritePointer(0), numSamples);

    buffer.copyFrom(0, 0, buffers[0], 0, 0, numSamples);
    buffer.copyFrom(1, 0, buffers[1], 0, 0, numSamples);
    buffer.copyFrom(2, 0, buffers[2], 0, 0, numSamples);
    buffer.addFrom (2, 0, buffers[3], 0, 0, numSamples);
    /**
     * Output channels:
     * 0. Lowpass
     * 1. Bandpass (centerFrequency, qValue)
     * 2. Highpass
     **/
}

void LRFilterBankProcessor::reset()
{
    A1_L.reset();
    A1_H.reset();
    A2_L.reset();
    A2_H.reset();
}
