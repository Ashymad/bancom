/*
  ==============================================================================

    IIRFilterCascadeProcessor.cpp
    Created: 6 Jan 2019 8:47:05pm
    Author:  shyman

  ==============================================================================
*/

#include "IIRFilterCascadeProcessor.h"

IIRFilterCascadeProcessor::IIRFilterCascadeProcessor() :
    cascade(OwnedArray<Filter>())
{
    setPlayConfigDetails(2, 2, getSampleRate(), getBlockSize());
}

IIRFilterCascadeProcessor::~IIRFilterCascadeProcessor()
{
}
void IIRFilterCascadeProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    setRateAndBufferSizeDetails(sampleRate, samplesPerBlock);
    dsp::ProcessSpec spec = { sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
    for (Filter* filter : cascade)
    {
	filter->prepare(spec);
    }
}
void IIRFilterCascadeProcessor::addFilterFromCoefficients(Coefficients::Ptr coefficients)
{
    Filter* filter = new Filter(coefficients);
    cascade.add(filter);
}
void IIRFilterCascadeProcessor::addFilterFromCoefficients(ReferenceCountedArray<Coefficients>& coefficients)
{
    for (Coefficients::Ptr coeffs : coefficients)
    {
	addFilterFromCoefficients(coeffs);
    }
}
void IIRFilterCascadeProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    for (Filter* filter : cascade)
    {
	dsp::AudioBlock<float> block (buffer);
	dsp::ProcessContextReplacing<float> context (block);
	filter->process(context);
    }
}
const String IIRFilterCascadeProcessor::getName() const
{
    return "IIRFilterCascadeProcessor";
}
void IIRFilterCascadeProcessor::reset()
{
    for (Filter* filter : cascade)
    {
	filter->reset();
    }
}
