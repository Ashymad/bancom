/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BancomAudioProcessor::BancomAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
	    .withInput  ("Input",  AudioChannelSet::stereo(), true)
#endif
	    .withOutput ("Output", AudioChannelSet::stereo(), true)
#endif
	    ),
#endif
    mainProcessor(new AudioProcessorGraph())
{
}

BancomAudioProcessor::~BancomAudioProcessor()
{

}

//==============================================================================
const String BancomAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BancomAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool BancomAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool BancomAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double BancomAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BancomAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int BancomAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BancomAudioProcessor::setCurrentProgram (int index)
{
}

const String BancomAudioProcessor::getProgramName (int index)
{
    return {};
}

void BancomAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BancomAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mainProcessor->setPlayConfigDetails (getMainBusNumInputChannels(),
	    getMainBusNumOutputChannels(),
	    sampleRate, samplesPerBlock);
    mainProcessor->prepareToPlay (sampleRate, samplesPerBlock);
    initialiseGraph();
}

void BancomAudioProcessor::releaseResources()
{
    mainProcessor->releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BancomAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet()  == AudioChannelSet::disabled()
	    || layouts.getMainOutputChannelSet() == AudioChannelSet::disabled())
    {
	return false;
    }
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
	    && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
    {
	return false;
    }
    return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
}
#endif

void BancomAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
	buffer.clear (i, 0, buffer.getNumSamples());
    updateGraph();
    mainProcessor->processBlock (buffer, midiMessages);
}

//==============================================================================
bool BancomAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BancomAudioProcessor::createEditor()
{
    return new BancomAudioProcessorEditor (*this);
}

//==============================================================================
void BancomAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BancomAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BancomAudioProcessor();
}
