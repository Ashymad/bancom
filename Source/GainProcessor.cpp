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

#include "GainProcessor.h"

GainProcessor::GainProcessor()
{
    setPlayConfigDetails(2, 2, getSampleRate(), getBlockSize());
    addParameter(gain = new AudioParameterFloat(
		"gain",
		"Gain",
		NormalisableRange<float>(0.0f, 40.0f),
		0.0f));
}

GainProcessor::~GainProcessor()
{
}

const String GainProcessor::getName() const
{
    return "GainProcessor";
}

void GainProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    setRateAndBufferSizeDetails(sampleRate, samplesPerBlock);
    dsp::ProcessSpec spec{sampleRate, static_cast<uint32>(samplesPerBlock), 2};
    gainDSP.prepare(spec);
}

void GainProcessor::reset()
{
    gainDSP.reset();
}

void GainProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    dsp::AudioBlock<float> block{buffer};
    dsp::ProcessContextReplacing<float> context{block};
    gainDSP.setGainDecibels(*gain);
    gainDSP.process(context);
}
