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
    attack(2.0f),
    release(100.0f),
    currentGain(0.0f),
    rmsValues(CircularArray<float>())
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
    int rmsValuesSize = 1 + static_cast<int>(averagingTime*sampleRate/samplesPerBlock);
    rmsValues.clear();
    rmsValues.insertMultiple(0, 0, rmsValuesSize);
}
void CompressorProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples();
    float sensorValue = (buffer.getRMSLevel(0, 0, numSamples) + buffer.getRMSLevel(1, 0, numSamples))/2;

    rmsValues.setAndRotate(square(sensorValue));
    sensorValue = 0;
    for (float el : rmsValues) sensorValue += el;
    sensorValue = Decibels::gainToDecibels(sqrt(sensorValue/rmsValues.size()));
    
    float compressGain = jmin(threshold - sensorValue, 0.0f)*(ratio-1)/ratio;
    float diffGain = compressGain - currentGain;

    if (abs(diffGain) > 0.1)
    {
	float rampSamples = getSampleRate()/gainSlope;
	rampSamples *= compressGain > currentGain ? diffGain*release : -diffGain*attack;
	int rampSamplesInt = roundToInt(rampSamples);
	if (numSamples < rampSamplesInt)
	{
	    compressGain = Decibels::gainToDecibels(Decibels::decibelsToGain(currentGain) +
		(Decibels::decibelsToGain(compressGain) - Decibels::decibelsToGain(currentGain))*numSamples/rampSamples);
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
void CompressorProcessor::setAttack(float newAttack)
{
    attack = newAttack;
}
void CompressorProcessor::setRelease(float newRelease)
{
    release = newRelease;
}
void CompressorProcessor::setRatio(float newRatio)
{
    ratio = newRatio;
}
void CompressorProcessor::setThreshold(float newThreshold)
{
    threshold = newThreshold;
}
void CompressorProcessor::reset()
{
}
