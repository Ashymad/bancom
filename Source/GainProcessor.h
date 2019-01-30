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

class GainProcessor  : public ProcessorBase
{
    public:
	GainProcessor();
	~GainProcessor();

	const String getName() const override;
	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
	void reset() override;

	void setGainDecibels(float newGainDecibels);
	float getGainDecibels() const;

    private:
	dsp::Gain<float> gain;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainProcessor)
};
