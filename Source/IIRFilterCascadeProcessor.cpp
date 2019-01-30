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

#include "IIRFilterCascadeProcessor.h"

IIRFilterCascadeProcessor::IIRFilterCascadeProcessor()
{
    setPlayConfigDetails(2, 2, getSampleRate(), getBlockSize());
}

IIRFilterCascadeProcessor::~IIRFilterCascadeProcessor()
{
}

const String IIRFilterCascadeProcessor::getName() const
{
    return "IIRFilterCascadeProcessor";
}

void IIRFilterCascadeProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    setRateAndBufferSizeDetails(sampleRate, samplesPerBlock);
    dsp::ProcessSpec spec{sampleRate, static_cast<uint32>(samplesPerBlock), 2};
    for (Filter* filter : cascade)
    {
	filter->prepare(spec);
    }
}

void IIRFilterCascadeProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    for (Filter* filter : cascade)
    {
	dsp::AudioBlock<float> block{buffer};
	dsp::ProcessContextReplacing<float> context{block};
	filter->process(context);
    }
}

void IIRFilterCascadeProcessor::reset()
{
    for (Filter* filter : cascade)
    {
	filter->reset();
    }
}

void IIRFilterCascadeProcessor::addFilterFromCoefficients(Coefficients::Ptr coefficients)
{
    Filter* filter = new Filter(coefficients);
    cascade.add(filter);
}

void IIRFilterCascadeProcessor::addFilterFromCoefficients(const ReferenceCountedArray<Coefficients>& coefficients)
{
    for (Coefficients::Ptr coeffs : coefficients)
    {
	addFilterFromCoefficients(coeffs);
    }
}
