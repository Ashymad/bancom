/* 
 * This file is part of the bancom distribution (https://github.com/Ashymad/bancom).
 * Copyright (c) 2019 Szymon Mikulicz.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "CompressorProcessor.h"

CompressorProcessor::CompressorProcessor() :
    ratio{1.0f},
    threshold{0.0f},
    attack{0.003f},
    release{0.1f},
    currentGain{0.0f},
    currentRMS{-90}
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
    currentRMS = Decibels::gainToDecibels(sqrt(sensorValue/rmsValues.size()));

    float compressGain = jmin(threshold - currentRMS, 0.0f)*(ratio-1)/ratio;
    float diffGain = compressGain - currentGain;

    if (abs(diffGain) >= minGainDifference)
    {
	currentGain = helpers::applyGainSlopeDecibels(buffer, currentGain, compressGain,
		slope/((diffGain > 0 ? release : attack)*getSampleRate()));
    }
    else
    {
	buffer.applyGain(Decibels::decibelsToGain(currentGain));
    }
}

void CompressorProcessor::reset()
{
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

void CompressorProcessor::setParametersFloat(const Array<float>& parameters)
{
    jassert(parameters.size() == 4);
    attack = parameters[0];
    release = parameters[1];
    ratio = parameters[3];
    threshold = parameters[4];
}

float CompressorProcessor::getRMS() const
{
    return currentRMS;
}

float CompressorProcessor::getCompression() const
{
    return currentGain;
}

Array<float> CompressorProcessor::getParametersFloat() const
{
    return Array<float>{attack, release, ratio, threshold};
}
