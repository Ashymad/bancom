/*
  ==============================================================================

    CompressorProcessor.cpp
    Created: 10 Jan 2019 7:35:49pm
    Author:  shyman

  ==============================================================================
*/

#include "CompressorProcessor.h"

CompressorProcessor::CompressorProcessor() :
    ratio(1.0f),
    threshold(0.0f),
    attack(0.0f),
    release(0.0f),
    currentGain(0.0f),
    gainSlope(6.0f)
{
    setPlayConfigDetails(2, 2, getSampleRate(), getBlockSize());
}
CompressorProcessor::~CompressorProcessor()
{
}
const String CompressorProcessor::getName() const
{
    return "CompressorProcessor";
}
void CompressorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    setRateAndBufferSizeDetails(sampleRate, samplesPerBlock);
}
void CompressorProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples();
    float sensorValue = Decibels::gainToDecibels(jmax(buffer.getRMSLevel(0, 0, numSamples), buffer.getRMSLevel(1, 0, numSamples)));
    
    float compressGain = jmin((threshold - sensorValue)/ratio, 0.0f);

    if (compressGain != currentGain)
    {
	float rampSamples = getSampleRate() * compressGain/gainSlope;
	rampSamples *= compressGain > currentGain ? attack : release;
	int rampSamplesInt = static_cast<int>(rampSamples);
	if (numSamples < rampSamplesInt)
	{
	    compressGain *= numSamples/rampSamples;
	    rampSamplesInt = numSamples;
	}
	buffer.applyGainRamp(0, rampSamplesInt,
		Decibels::decibelsToGain(currentGain),
		Decibels::decibelsToGain(compressGain));
	currentGain = compressGain;
    }
    else
    {
	buffer.applyGain(Decibels::decibelsToGain(currentGain));
    }
}
void CompressorProcessor::reset()
{
}
