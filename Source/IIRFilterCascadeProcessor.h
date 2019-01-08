/*
  ==============================================================================

    IIRFilterCascade.h
    Created: 6 Jan 2019 8:47:05pm
    Author:  shyman

  ==============================================================================
*/

#pragma once

#include "ProcessorBase.h"

class IIRFilterCascadeProcessor : public ProcessorBase
{
    public:
	IIRFilterCascadeProcessor();
	void addFilter(IIRFilter& filter);
	void addFilter(Array<IIRFilter>& filters);
	void addFilterFromCoefficients(IIRCoefficients& coefficients);
	void addFilterFromCoefficients(ReferenceCountedArray<IIRCoefficients> coefficients);
	const String getName() const override;
	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
	void reset() override;
    private:
	Array<IIRFilter> cascade;
};
