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
    gainNodes(Array<Node::Ptr>()),
    compressorNodes(Array<Node::Ptr>())
{
    initialiseGraph();
    frequencies = Array<float>(125.0f);
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
    
    initialiseFilters(sampleRate);
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

void BancomAudioProcessor::initialiseFilters(float sampleRate)
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
	compressorNodes.add(mainProcessor->addNode(new CompressorProcessor()));
    }
    while (filterNodes.size() < gainNodes.size())
    {
	mainProcessor->removeNode(gainNodes.removeAndReturn(gainNodes.size()-1)->nodeID);
	mainProcessor->removeNode(compressorNodes.removeAndReturn(compressorNodes.size()-1)->nodeID);
    }
}

void BancomAudioProcessor::setFrequencies(const Array<float>& frequencies)
{
    this->frequencies = frequencies;
}

void BancomAudioProcessor::initialiseGraph()
{
    mainProcessor->clear();
    filterNodes.clear();
    gainNodes.clear();
    compressorNodes.clear();

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
	    mainProcessor->addConnection({ { filterNodes[node]->nodeID, channel}, { compressorNodes[node]->nodeID, channel } });
	    mainProcessor->addConnection({ { compressorNodes[node]->nodeID, channel}, { gainNodes[node]->nodeID, channel } });
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

void BancomAudioProcessor::setAttackOnFilter(unsigned int filterNumber, float newAttack)
{
    if (filterNumber < compressorNodes.size()){
	CompressorProcessor* compressorProcessor = dynamic_cast<CompressorProcessor*>(compressorNodes[filterNumber]->getProcessor());
	compressorProcessor->setAttack(newAttack);
    }
}
void BancomAudioProcessor::setRatioOnFilter(unsigned int filterNumber, float newRatio)
{
    if (filterNumber < compressorNodes.size()){
	CompressorProcessor* compressorProcessor = dynamic_cast<CompressorProcessor*>(compressorNodes[filterNumber]->getProcessor());
	compressorProcessor->setRatio(newRatio);
    }
}
void BancomAudioProcessor::setThresholdOnFilter(unsigned int filterNumber, float newThreshold)
{
    if (filterNumber < compressorNodes.size()){
	CompressorProcessor* compressorProcessor = dynamic_cast<CompressorProcessor*>(compressorNodes[filterNumber]->getProcessor());
	compressorProcessor->setThreshold(newThreshold);
    }
}
void BancomAudioProcessor::setReleaseOnFilter(unsigned int filterNumber, float newRelease)
{
    if (filterNumber < compressorNodes.size()){
	CompressorProcessor* compressorProcessor = dynamic_cast<CompressorProcessor*>(compressorNodes[filterNumber]->getProcessor());
	compressorProcessor->setRelease(newRelease);
    }
}
float BancomAudioProcessor::getRMSOnFilter(unsigned int filterNumber)
{
    if (filterNumber < compressorNodes.size()){
	CompressorProcessor* compressorProcessor = dynamic_cast<CompressorProcessor*>(compressorNodes[filterNumber]->getProcessor());
	return compressorProcessor->getRMS();
    } else return -90;
}
float BancomAudioProcessor::getCompressionOnFilter(unsigned int filterNumber)
{
    if (filterNumber < compressorNodes.size()){
	CompressorProcessor* compressorProcessor = dynamic_cast<CompressorProcessor*>(compressorNodes[filterNumber]->getProcessor());
	return compressorProcessor->getCompression();
    } else return 0;
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
    ValueTree tree = ValueTree("BancomState");

    for (int i = 0; i < compressorNodes.size(); ++i)
    {
	ValueTree bTree = ValueTree(Identifier("Band" + std::to_string(i)));
	CompressorProcessor* compressorProcessor = dynamic_cast<CompressorProcessor*>(compressorNodes[i]->getProcessor());
	Array<float> compressorParameters = compressorProcessor->getParametersFloat();
	GainProcessor* gainProcessor = dynamic_cast<GainProcessor*>(gainNodes[i]->getProcessor());

	if (i > 0) bTree.setProperty("frequency", frequencies[i-1], nullptr);
	bTree.setProperty("gain", gainProcessor->getGainDecibels(), nullptr);
	bTree.setProperty("attack", compressorParameters[0], nullptr);
	bTree.setProperty("release", compressorParameters[1], nullptr);
	bTree.setProperty("ratio", compressorParameters[2], nullptr);
	bTree.setProperty("threshold", compressorParameters[3], nullptr);
	tree.appendChild(bTree, nullptr);
    }

    MemoryOutputStream stream(destData, false);
    tree.writeToStream(stream);
}

void BancomAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ValueTree tree = ValueTree::readFromData (data, size_t (sizeInBytes));
    if (tree.isValid()) {
	gainNodes.clear();
	compressorNodes.clear();
	frequencies.clear();

	for (int i = 0; i < tree.getNumChildren(); ++i)
	{
	    ValueTree bTree = tree.getChild(i);
	    CompressorProcessor*  compressorProcessor = new CompressorProcessor();
	    GainProcessor* gainProcessor = new GainProcessor();

	    if (i > 0) frequencies.add(bTree.getProperty("frequency"));

	    float gain = bTree.getProperty("gain");
	    float attack = bTree.getProperty("attack");
	    float release = bTree.getProperty("release");
	    float ratio = bTree.getProperty("ratio");
	    float threshold = bTree.getProperty("threshold");

	    gainProcessor->setGainDecibels(gain);
	    compressorProcessor->setAttack(attack*1000);
	    compressorProcessor->setRelease(release*1000);
	    compressorProcessor->setRatio(ratio);
	    compressorProcessor->setThreshold(threshold);

	    gainNodes.add(mainProcessor->addNode(gainProcessor));
	    compressorNodes.add(mainProcessor->addNode(compressorProcessor));
	}
    }
}

void BancomAudioProcessor::reset()
{
    mainProcessor->reset();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BancomAudioProcessor();
}
