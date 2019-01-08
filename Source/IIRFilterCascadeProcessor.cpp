/*
  ==============================================================================

    IIRFilterCascadeProcessor.cpp
    Created: 6 Jan 2019 8:47:05pm
    Author:  shyman

  ==============================================================================
*/

#include "IIRFilterCascadeProcessor.h"

IIRFilterCascadeProcessor::IIRFilterCascadeProcessor() :
    cascade(Array<IIRFilter>())
{
}
void IIRFilterCascadeProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{

}
void IIRFilterCascadeProcessor::addFilter(IIRFilter& filter)
{
    cascade.add(filter);
}
void IIRFilterCascadeProcessor::addFilter(Array<IIRFilter>& filters)
{
    cascade.addArray(filters);
}
void IIRFilterCascadeProcessor::addFilterFromCoefficients(IIRCoefficients& coefficients)
{
    IIRFilter filter = IIRFilter();
    filter.setCoefficients(coefficients);
    cascade.add(filter);
}
void IIRFilterCascadeProcessor::addFilterFromCoefficients(ReferenceCountedArray<IIRCoefficients> coefficients)
{
    for (IIRCoefficients* coeffs : coefficients)
    {
	addFilterFromCoefficients(*coeffs);
    }
}
void IIRFilterCascadeProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples();
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
	auto wPointer = buffer.getWritePointer(channel);
	for (IIRFilter& filter : cascade)
	{
	    filter.processSamples(wPointer, numSamples);
	}
    }
}
void IIRFilterCascadeProcessor::reset()
{
    for (IIRFilter& filter : cascade)
    {
	filter.reset();
    }
}
