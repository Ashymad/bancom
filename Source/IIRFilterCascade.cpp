/*
  ==============================================================================

    IIRFilterCascade.cpp
    Created: 6 Jan 2019 8:47:05pm
    Author:  shyman

  ==============================================================================
*/

#include "IIRFilterCascade.h"

IIRFilterCascade::IIRFilterCascade() :
    cascade(Array<IIRFilter>())
{
}
void IIRFilterCascade::addFilter(IIRFilter& filter)
{
    cascade.add(filter);
}
void IIRFilterCascade::addFilterFromCoefficients(IIRCoefficients& coefficients)
{
    IIRFilter filter = IIRFilter();
    filter.setCoefficients(coefficients);
    cascade.add(filter);
}
void IIRFilterCascade::addFilterFromCoefficients(ReferenceCountedArray<IIRCoefficients> coefficients)
{
    for (IIRCoefficients* coeffs : coefficients)
    {
	addFilterFromCoefficients(*coeffs);
    }
}
void IIRFilterCascade::processSamples(float* samples, int numSamples) noexcept
{
    for (IIRFilter& filter : cascade){
	filter.processSamples(samples, numSamples);
    }
}
float IIRFilterCascade::processSingleSampleRaw(float sample) noexcept
{
    for (IIRFilter& filter : cascade){
	sample = filter.processSingleSampleRaw(sample);
    }
    return sample;
}
void IIRFilterCascade::reset()
{
    for (IIRFilter& filter : cascade){
	filter.reset();
    }
}
