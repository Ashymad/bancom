/*
  ==============================================================================

    IIRFilterCascade.h
    Created: 6 Jan 2019 8:47:05pm
    Author:  shyman

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class IIRFilterCascade
{
    public:
	IIRFilterCascade();
	void addFilter(IIRFilter& filter);
	void addFilterFromCoefficients(IIRCoefficients& coefficients);
	void addFilterFromCoefficients(ReferenceCountedArray<IIRCoefficients> coefficients);
	void processSamples(float* samples, int numSamples) noexcept;
	float processSingleSampleRaw(float sample) noexcept;
	void reset();
    private:
	Array<IIRFilter> cascade;
};
