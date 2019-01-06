/*
  ==============================================================================

    LRFilterBankProcessor.h
    Created: 6 Jan 2019 7:46:28pm
    Author:  shyman

  ==============================================================================
*/

#pragma once

#include "ProcessorBase.h"
#include "helpers.h"
#include "IIRFilterCascade.h"

class LRFilterBankProcessor  : public ProcessorBase
{
    public:
	LRFilterBankProcessor();
	const String getName() const override;
	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
	void reset() override;
    private:
	float qValue;
	float centerFrequency;
	int order;
	IIRFilterCascade A1_L;
	IIRFilterCascade A1_H;
	IIRFilterCascade A2_L;
	IIRFilterCascade A2_H;
};
