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

#include "helpers.h"

template<typename FloatType>
ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>>
helpers::designIIRLowpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order)
{
    jassert(order % 2 == 0);
    ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>> coeff(
	    dsp::FilterDesign<FloatType>::designIIRLowpassHighOrderButterworthMethod(frequency, sampleRate, order/2));
    coeff.addArray(ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>>(coeff));
    return coeff;
}

template<typename FloatType>
ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>>
helpers::designIIRHighpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order)
{
    jassert(order % 2 == 0);
    ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>> coeff(
	    dsp::FilterDesign<FloatType>::designIIRHighpassHighOrderButterworthMethod(frequency, sampleRate, order/2));
    coeff.addArray(ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>>(coeff));
    return coeff;
}

OwnedArray<IIRFilterCascadeProcessor>
helpers::designLRFilterBank(const Array<float>& frequencies, double sampleRate, int order)
{
    int crosses = frequencies.size();

    jassert([&](){// the frequencies array must be sorted
	auto sorted = Array<float>(frequencies);
	sorted.sort();
	return sorted == frequencies;
    }());

    std::vector<ReferenceCountedArray<dsp::IIR::Coefficients<float>>> LPs{};
    std::vector<ReferenceCountedArray<dsp::IIR::Coefficients<float>>> HPs{};
    OwnedArray<IIRFilterCascadeProcessor> filterBank{};
    filterBank.add(new IIRFilterCascadeProcessor());

    for (int i = frequencies.size() - 1; i >= 0; --i)
    {
	LPs.push_back(designIIRLowpassHighOrderLRMethod(frequencies[i], sampleRate, order));
	HPs.push_back(designIIRHighpassHighOrderLRMethod(frequencies[i], sampleRate, order));
	filterBank.add(new IIRFilterCascadeProcessor());
    }
    
    for (int processor = 0; processor <= crosses; ++processor)
    {
	for (int filter = 0; filter < crosses; ++filter)
	{
	    filterBank[processor]->addFilterFromCoefficients(processor > filter ? LPs[filter] : HPs[filter]);
	}
    }

    return filterBank;
}

float helpers::applyGainSlopeDecibels(AudioSampleBuffer& buffer, float startGainDecibels,
	float endGainDecibels, float slopeDecibels, int startIndex) // slope is in decibels per sample
{
    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    jassert(startIndex < numSamples);
    jassert(slopeDecibels >= 0);
    jassert(startIndex >= 0);

    float endGain = Decibels::decibelsToGain(endGainDecibels);
    int sign = endGainDecibels > startGainDecibels ?  1 : -1;
    float slope = Decibels::decibelsToGain(sign*slopeDecibels);
    float currentGain;
    float* wPointer;

    for (int channel = 0; channel < numChannels; ++channel)
    {
	wPointer = buffer.getWritePointer(channel);
	currentGain = Decibels::decibelsToGain(startGainDecibels);
	for (int sample = startIndex; sample < numSamples; ++sample)
	{
	    if (sign*currentGain < sign*endGain)
	    {
		currentGain *= slope;
	    }
	    wPointer[sample] *= currentGain;
	}
    }
    return Decibels::gainToDecibels(currentGain);
}
