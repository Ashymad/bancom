/*
  ==============================================================================

    CompressorProcessor.h
    Created: 10 Jan 2019 7:35:49pm
    Author:  shyman

  ==============================================================================
*/

#pragma once

#include "ProcessorBase.h"

class CompressorProcessor  : public ProcessorBase
{
    public:
	CompressorProcessor();
	~CompressorProcessor();
	const String getName() const override;
	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
	void reset() override;
    private:
	float currentGain;
	const float gainSlope;
	float ratio, threshold, attack, release;
	/**
	 * Ratio - RMS [dB] over threshold is divided by this value to obtain the gain
	 * Threshold - threshold value in dB
	 * Attack - time it takes to increase gain SLOPE dB
	 * Release - time it takes to decrease gain SLOPE dB
	 **/
};
