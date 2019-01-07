/*
  ==============================================================================

    SumProcessor.h
    Created: 7 Jan 2019 10:41:04am
    Author:  shyman

  ==============================================================================
*/

#pragma once

#include "ProcessorBase.h"

class SumProcessor  : public ProcessorBase
{
    public:
	const String getName() const override;
	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
};
