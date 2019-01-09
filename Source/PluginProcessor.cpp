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
    mainProcessor(new AudioProcessorGraph()),
    filterNodes(Array<Node::Ptr>()),
    gainNodes(Array<Node::Ptr>())
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
    return false;
}

bool BancomAudioProcessor::producesMidi() const
{
    return false;
}

bool BancomAudioProcessor::isMidiEffect() const
{
    return false;
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
    
    Array<float> frequencies = Array<float>(250.0f);
    initialiseGraph();
    initialiseFilters(frequencies, sampleRate);
    prepareGraph(sampleRate, samplesPerBlock);
    connectNodes();
}

void BancomAudioProcessor::prepareGraph (double sampleRate, int samplesPerBlock)
{
    sampleRate = sampleRate == 0 ? getSampleRate() : sampleRate;
    samplesPerBlock = samplesPerBlock == 0 ? getBlockSize() : samplesPerBlock;

    mainProcessor->prepareToPlay(sampleRate, samplesPerBlock);
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
    mainProcessor->processBlock (buffer, midiMessages);
}

void BancomAudioProcessor::initialiseFilters(Array<float>& frequencies, float sampleRate)
{
    sampleRate = sampleRate == 0 ? getSampleRate() : sampleRate;

    for (Node::Ptr filterNode : filterNodes){
	mainProcessor->removeNode(filterNode->nodeID);
    }

    filterNodes.clear();

    auto filterBank = designLRFilterBank(frequencies, sampleRate, 4);

    while (filterBank.size() > 0)
    {
	filterNodes.add(mainProcessor->addNode(filterBank.removeAndReturn(filterBank.size() - 1)));
    }

    while (filterNodes.size() > gainNodes.size())
    {
	gainNodes.add(mainProcessor->addNode(new GainProcessor()));
    }
    while (filterNodes.size() < gainNodes.size())
    {
	mainProcessor->removeNode(gainNodes.removeAndReturn(gainNodes.size()-1)->nodeID);
    }

    DBG("Filter bank designed");
    DBG(gainNodes.size());
}

void BancomAudioProcessor::initialiseGraph()
{
    mainProcessor->clear();

    audioInputNode  = mainProcessor->addNode (new AudioGraphIOProcessor (AudioGraphIOProcessor::audioInputNode));
    audioOutputNode = mainProcessor->addNode (new AudioGraphIOProcessor (AudioGraphIOProcessor::audioOutputNode));
}

void BancomAudioProcessor::connectNodes()
{
    for (AudioProcessorGraph::Connection& connection : mainProcessor->getConnections())
    {
	mainProcessor->removeConnection(connection);
    }
    for (int channel = 0; channel < 2; ++channel)
    {
	for (int node = 0; node < filterNodes.size(); ++node)
	{
	    mainProcessor->addConnection({ { audioInputNode->nodeID,  channel }, { filterNodes[node]->nodeID, channel } });
	    mainProcessor->addConnection({ { filterNodes[node]->nodeID, channel}, { gainNodes[node]->nodeID, channel } });
	    mainProcessor->addConnection({ { gainNodes[node]->nodeID, channel}, { audioOutputNode->nodeID, channel } });
	}
    }

}

void BancomAudioProcessor::setGainOnFilter(unsigned int filterNumber, float newGainDecibels)
{
    if (filterNumber < gainNodes.size()){
	GainProcessor* gainProcessor = dynamic_cast<GainProcessor*>(gainNodes[filterNumber]->getProcessor());
	gainProcessor->setGainDecibels(newGainDecibels);
    }
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
