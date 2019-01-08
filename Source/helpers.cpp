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
    coeff.addArray(coeff);
    return coeff;
}

template<typename FloatType>
ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>> designIIRHighpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order)
{
    jassert(order % 2 == 0);
    ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>> coeff = dsp::FilterDesign<FloatType>::designIIRHighpassHighOrderButterworthMethod(frequency, sampleRate, order/2);
    coeff.addArray(coeff);
    return coeff;
}

OwnedArray<IIRFilterCascadeProcessor> designLRFilterBank(Array<float>& frequencies, double sampleRate, int order)
{
    int crosses = frequencies.size();

    OwnedArray<ReferenceCountedArray<dsp::IIR::Coefficients<float>>> LPs;
    OwnedArray<ReferenceCountedArray<dsp::IIR::Coefficients<float>>> HPs;
    OwnedArray<IIRFilterCascadeProcessor> filterBank = OwnedArray<IIRFilterCascadeProcessor>();
    filterBank.add(new IIRFilterCascadeProcessor());

    frequencies.sort();
    
    for (float frequency : frequencies)
    {
	LPs.add(new ReferenceCountedArray<dsp::IIR::Coefficients<float>>(designIIRLowpassHighOrderLRMethod(frequency, sampleRate, order)));
	HPs.add(new ReferenceCountedArray<dsp::IIR::Coefficients<float>>(designIIRHighpassHighOrderLRMethod(frequency, sampleRate, order)));
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
