/*
  ==============================================================================

    helpers.h
    Created: 6 Jan 2019 1:45:37pm
    Author:  shyman

  ==============================================================================
*/

#pragma once

#include "IIRFilterCascadeProcessor.h"

namespace helpers {
    template<typename FloatType>
    ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>>
    designIIRLowpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order);

    template<typename FloatType>
    ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>>
    designIIRHighpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order);

    OwnedArray<IIRFilterCascadeProcessor>
    designLRFilterBank(Array<float>& frequencies, double sampleRate, int order);

    float applyGainSlopeDecibels(AudioSampleBuffer& buffer, float startGainDecibels,
	    float endGainDecibles, float slopeDecibels, int startIndex=0);
}
