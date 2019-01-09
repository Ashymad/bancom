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
	~IIRFilterCascadeProcessor();
	void addFilter(dsp::IIR::Filter<float>* filter);
	void addFilterFromCoefficients(dsp::IIR::Coefficients<float>::Ptr coefficients);
	void addFilterFromCoefficients(ReferenceCountedArray<dsp::IIR::Coefficients<float>>& coefficients);
	const String getName() const override;
	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
	void reset() override;
    private:
	OwnedArray<dsp::IIR::Filter<float>> cascade;
	dsp::ProcessSpec spec;
};
