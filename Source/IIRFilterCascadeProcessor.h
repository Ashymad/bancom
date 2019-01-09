/*
  ==============================================================================

    IIRFilterCascade.h
    Created: 6 Jan 2019 8:47:05pm
    Author:  shyman

  ==============================================================================
*/

#pragma once

#include "ProcessorBase.h"
using Coefficients = dsp::IIR::Coefficients<float>;
using Filter = dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, Coefficients>;

class IIRFilterCascadeProcessor : public ProcessorBase
{
    public:
	IIRFilterCascadeProcessor();
	~IIRFilterCascadeProcessor();
	void addFilterFromCoefficients(Coefficients::Ptr coefficients);
	void addFilterFromCoefficients(ReferenceCountedArray<Coefficients>& coefficients);
	const String getName() const override;
	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
	void reset() override;
    private:
	OwnedArray<Filter> cascade;
};
