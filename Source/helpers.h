/*
  ==============================================================================

    helpers.h
    Created: 6 Jan 2019 1:45:37pm
    Author:  shyman

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

template<typename FloatType>
ReferenceCountedArray<IIRCoefficients> designIIRLowpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order);

template<typename FloatType>
ReferenceCountedArray<IIRCoefficients> designIIRHighpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order);
