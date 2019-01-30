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
    setPlayConfigDetails(2, 2, getSampleRate(), getBlockSize());
    gain.setGainDecibels (0.0f);
}

GainProcessor::~GainProcessor()
{
}

const String GainProcessor::getName() const
{
    return "Gain";
}

void GainProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    setRateAndBufferSizeDetails(sampleRate, samplesPerBlock);
    dsp::ProcessSpec spec{sampleRate, static_cast<uint32>(samplesPerBlock), 2};
    gain.prepare(spec);
}

void GainProcessor::reset()
{
    gain.reset();
}

void GainProcessor::setGainDecibels(float newGainDecibels)
{
    gain.setGainDecibels(newGainDecibels);
}

float GainProcessor::getGainDecibels() const
{
    return gain.getGainDecibels();
}

void GainProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    dsp::AudioBlock<float> block{buffer};
    dsp::ProcessContextReplacing<float> context{block};
    gain.process(context);
}
