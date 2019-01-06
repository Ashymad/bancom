/*
  ==============================================================================

    GainProcessor.cpp
    Created: 6 Jan 2019 7:24:02pm
    Author:  shyman

  ==============================================================================
*/

#include "GainProcessor.h"

GainProcessor::GainProcessor()
{
    gain.setGainDecibels (0.0f);
}

const String GainProcessor::getName() const
{
    return "Gain";
}

void GainProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec { sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
    gain.prepare (spec);
}

void GainProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    dsp::AudioBlock<float> block (buffer);
    dsp::ProcessContextReplacing<float> context (block);
    gain.process (context);
}

void GainProcessor::reset()
{
    gain.reset();
}
