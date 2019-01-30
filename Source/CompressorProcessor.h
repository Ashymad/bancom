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

#pragma once

#include "ProcessorBase.h"
#include "CircularArray.h"
#include "helpers.h"

class CompressorProcessor  : public ProcessorBase
{
    public:
	CompressorProcessor();
	~CompressorProcessor();

	const String getName() const override;
	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
	void reset() override;

	void setAttack(float newAttack);
	void setRelease(float newRelease);
	void setRatio(float newRatio);
	void setThreshold(float newThreshold);
	void setParametersFloat(const Array<float>& parameters);

	float getRMS() const;
	float getCompression() const;
	Array<float> getParametersFloat() const;

    private:
	const float slope = 6;
	const float averagingTime = 0.5;
	const float minGainDifference = 0.01;

	float currentGain;
	float currentRMS;
	float squareSum;
	CircularArray<float> squareValues;

	float ratio, threshold, attack, release;
	/**
	 * Ratio - RMS [dB] over threshold is divided by this value to obtain the gain
	 * Threshold - threshold value in dB
	 * Attack - time it takes to increase gain by $slope dB
	 * Release - time it takes to decrease gain by $slope dB
	 **/

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorProcessor)
};
