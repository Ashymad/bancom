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
