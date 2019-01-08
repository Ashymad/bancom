/*
  ==============================================================================

    helpers.cpp
    Created: 6 Jan 2019 1:45:37pm
    Author:  shyman

  ==============================================================================
*/

#include "helpers.h"

template<typename FloatType>
ReferenceCountedArray<IIRCoefficients> designIIRLowpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order)
{
    jassert(order % 2 == 0);
    ReferenceCountedArray<IIRCoefficients> coeff = dsp::FilterDesign<FloatType>::designIIRLowpassHighOrderButterworthMethod(frequency, sampleRate, order/2);
    coeff.addArray(coeff);
    return coeff;
}

template<typename FloatType>
ReferenceCountedArray<IIRCoefficients> designIIRHighpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order)
{
    jassert(order % 2 == 0);
    ReferenceCountedArray<IIRCoefficients> coeff = dsp::FilterDesign<FloatType>::designIIRHighpassHighOrderButterworthMethod(frequency, sampleRate, order/2);
    coeff.addArray(coeff);
    return coeff;
}

template<typename FloatType>
ReferenceCountedArray<IIRFilterCascadeProcessor> designLRFilterBank(Array<FloatType> frequencies, double sampleRate, int order)
{
    int crosses = frequencies.size();

    Array<ReferenceCountedArray<IIRCoefficients>> LPs;
    Array<ReferenceCountedArray<IIRCoefficients>> HPs;
    ReferenceCountedArray<IIRFilterCascadeProcessor> filterBank = ReferenceCountedArray<IIRFilterCascadeProcessor>();
    filterBank.add(new IIRFilterCascadeProcessor());

    frequencies.sort();
    
    for (FloatType frequency : frequencies)
    {
	LPs.add(designIIRLowpassHighOrderLRMethod(frequency, sampleRate, order));
	HPs.add(designIIRHighpassHighOrderLRMethod(frequency, sampleRate, order));
	filterBank.add(new IIRFilterCascadeProcessor());
    }
    
    for (int processor = 0; processor <= crosses; ++processor)
    {
	for (int filter = 0; filter < crosses; ++filter)
	{
	    filterBank[processor]->addFilterFromCoefficients(processor > filter ? LPs[filter] : HPs[filter]);
	}
    }

    return filterBank;
}
