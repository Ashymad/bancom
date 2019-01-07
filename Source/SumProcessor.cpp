/*
  ==============================================================================

    SumProcessor.cpp
    Created: 7 Jan 2019 10:41:04am
    Author:  shyman

  ==============================================================================
*/

#include "SumProcessor.h"

const String SumProcessor::getName() const
{
    return "Sum";
}

void SumProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples();
    for (int i = 1; i < buffer.getNumChannels(); i++)
    {
	buffer.addFrom(0, 0, buffer, i, 0, numSamples);
    }
}
