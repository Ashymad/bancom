/*
  ==============================================================================

    GainProcessor.h
    Created: 6 Jan 2019 7:24:02pm
    Author:  shyman

  ==============================================================================
*/

#pragma once

#include "ProcessorBase.h"

class GainProcessor  : public ProcessorBase
{
    public:
	GainProcessor();
	~GainProcessor();
	const String getName() const override;
	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
	void setGainDecibels(float newGainDecibels);
	float getGainDecibels();
	void reset() override;
    private:
	dsp::Gain<float> gain;
};
