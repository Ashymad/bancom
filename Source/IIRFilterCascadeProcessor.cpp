/*
  ==============================================================================

    IIRFilterCascadeProcessor.cpp
    Created: 6 Jan 2019 8:47:05pm
    Author:  shyman

  ==============================================================================
*/

#include "IIRFilterCascadeProcessor.h"

IIRFilterCascadeProcessor::IIRFilterCascadeProcessor() :
    cascade(OwnedArray<dsp::IIR::Filter<float>>())
{
}

IIRFilterCascadeProcessor::~IIRFilterCascadeProcessor()
{
}
void IIRFilterCascadeProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    spec = { sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
}
void IIRFilterCascadeProcessor::addFilter(dsp::IIR::Filter<float>& filter)
{
    cascade.add(&filter);
}
void IIRFilterCascadeProcessor::addFilterFromCoefficients(dsp::IIR::Coefficients<float>::Ptr coefficients)
{
    dsp::IIR::Filter<float>*filter = new dsp::IIR::Filter<float>(coefficients);
    filter->prepare(spec);
    cascade.add(filter);
}
void IIRFilterCascadeProcessor::addFilterFromCoefficients(ReferenceCountedArray<dsp::IIR::Coefficients<float>>& coefficients)
{
    for (dsp::IIR::Coefficients<float>::Ptr coeffs : coefficients)
    {
	addFilterFromCoefficients(coeffs);
    }
}
void IIRFilterCascadeProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    for (dsp::IIR::Filter<float>* filter : cascade)
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
    for (dsp::IIR::Filter<float>* filter : cascade)
    {
	filter->reset();
    }
}