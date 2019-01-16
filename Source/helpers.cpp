/*
  ==============================================================================

    helpers.cpp
    Created: 6 Jan 2019 1:45:37pm
    Author:  shyman

  ==============================================================================
*/

#include "helpers.h"

template<typename FloatType>
ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>> designIIRLowpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order)
{
    jassert(order % 2 == 0);
    ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>> coeff = dsp::FilterDesign<FloatType>::designIIRLowpassHighOrderButterworthMethod(frequency, sampleRate, order/2);
    coeff.addArray(ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>>(coeff));
    return coeff;
}

template<typename FloatType>
ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>> designIIRHighpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order)
{
    jassert(order % 2 == 0);
    ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>> coeff = dsp::FilterDesign<FloatType>::designIIRHighpassHighOrderButterworthMethod(frequency, sampleRate, order/2);
    coeff.addArray(ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>>(coeff));
    return coeff;
}

OwnedArray<IIRFilterCascadeProcessor> designLRFilterBank(Array<float>& frequencies, double sampleRate, int order)
{
    int crosses = frequencies.size();

    Array<ReferenceCountedArray<dsp::IIR::Coefficients<float>>*> LPs;
    Array<ReferenceCountedArray<dsp::IIR::Coefficients<float>>*> HPs;
    OwnedArray<IIRFilterCascadeProcessor> filterBank = OwnedArray<IIRFilterCascadeProcessor>();
    filterBank.add(new IIRFilterCascadeProcessor());

    frequencies.sort();
    
    for (int i = frequencies.size() - 1; i >= 0; --i)
    {
	LPs.add(new ReferenceCountedArray<dsp::IIR::Coefficients<float>>(designIIRLowpassHighOrderLRMethod(frequencies[i], sampleRate, order)));
	HPs.add(new ReferenceCountedArray<dsp::IIR::Coefficients<float>>(designIIRHighpassHighOrderLRMethod(frequencies[i], sampleRate, order)));
	filterBank.add(new IIRFilterCascadeProcessor());
    }
    
    for (int processor = 0; processor <= crosses; ++processor)
    {
	for (int filter = 0; filter < crosses; ++filter)
	{
	    filterBank[processor]->addFilterFromCoefficients(processor > filter ? *LPs[filter] : *HPs[filter]);
	}
    }

    return filterBank;
}

float applyGainSlopeDecibels(AudioSampleBuffer& buffer,
	float startGainDecibels, float endGainDecibels, float slopeDecibels,
	unsigned int startIndex) // slope is in decibels per sample
{
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    jassert(startIndex < numSamples);
    jassert(slopeDecibels >= 0);

    float endGain = Decibels::decibelsToGain(endGainDecibels);
    int sign = endGainDecibels > startGainDecibels ?  1 : -1;
    float slope = Decibels::decibelsToGain(sign*slopeDecibels);
    float currentGain;
    float* wPointer;

    for (int channel = 0; channel < numChannels; ++channel)
    {
	wPointer = buffer.getWritePointer(channel);
	currentGain = Decibels::decibelsToGain(startGainDecibels);
	for (int sample = startIndex; sample < numSamples; ++sample)
	{
	    if (sign*currentGain < sign*endGain) currentGain *= slope;
	    wPointer[sample] *= currentGain;
	}
    }
    return Decibels::gainToDecibels(currentGain);
}
