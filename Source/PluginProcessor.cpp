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

#include "PluginProcessor.h"
#include "PluginEditor.h"

BancomAudioProcessor::BancomAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
    ProcessorBase (BusesProperties()
	    .withInput  ("Input",  AudioChannelSet::stereo(), true)
	    .withOutput ("Output", AudioChannelSet::stereo(), true)
	    ),
#endif
    mainProcessor{new AudioProcessorGraph()}
{
    frequencies = Array<float>(125.0f);
    initialiseGraph();
}

BancomAudioProcessor::~BancomAudioProcessor()
{

}

const String BancomAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

void BancomAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mainProcessor->setPlayConfigDetails(getMainBusNumInputChannels(),
	    getMainBusNumOutputChannels(),
	    sampleRate, samplesPerBlock);

    initialiseFilters(sampleRate);
    prepareGraph(sampleRate, samplesPerBlock);
    connectNodes();
}

void BancomAudioProcessor::releaseResources()
{
    mainProcessor->releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BancomAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() == AudioChannelSet::disabled()
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

bool BancomAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BancomAudioProcessor::createEditor()
{
    return new BancomAudioProcessorEditor{*this};
}

void BancomAudioProcessor::initialiseGraph()
{
    mainProcessor->clear();
    filterNodes.clear();
    gainNodes.clear();
    compressorNodes.clear();

    audioInputNode  = mainProcessor->addNode(new AudioGraphIOProcessor (AudioGraphIOProcessor::audioInputNode));
    audioOutputNode = mainProcessor->addNode(new AudioGraphIOProcessor (AudioGraphIOProcessor::audioOutputNode));
}

void BancomAudioProcessor::initialiseFilters(float sampleRate)
{
    sampleRate = sampleRate == 0 ? getSampleRate() : sampleRate;

    for (Node::Ptr filterNode : filterNodes){
	mainProcessor->removeNode(filterNode->nodeID);
    }

    filterNodes.clear();

    auto filterBank = helpers::designLRFilterBank(frequencies, sampleRate, 4);

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

void BancomAudioProcessor::prepareGraph (double sampleRate, int samplesPerBlock)
{
    sampleRate = sampleRate == 0 ? getSampleRate() : sampleRate;
    samplesPerBlock = samplesPerBlock == 0 ? getBlockSize() : samplesPerBlock;

    mainProcessor->prepareToPlay(sampleRate, samplesPerBlock);
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
	    mainProcessor->addConnection({{audioInputNode->nodeID,  channel}, {filterNodes[node]->nodeID, channel}});
	    mainProcessor->addConnection({{filterNodes[node]->nodeID, channel}, {compressorNodes[node]->nodeID, channel}});
	    mainProcessor->addConnection({{compressorNodes[node]->nodeID, channel}, {gainNodes[node]->nodeID, channel}});
	    mainProcessor->addConnection({{gainNodes[node]->nodeID, channel}, {audioOutputNode->nodeID, channel}});
	}
    }

}

void BancomAudioProcessor::setFrequencies(const Array<float>& frequencies)
{
    this->frequencies = frequencies;
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
float BancomAudioProcessor::getRMSOnFilter(unsigned int filterNumber) const
{
    if (filterNumber < compressorNodes.size()){
	CompressorProcessor* compressorProcessor = dynamic_cast<CompressorProcessor*>(compressorNodes[filterNumber]->getProcessor());
	return compressorProcessor->getRMS();
    } else return -90;
}
float BancomAudioProcessor::getCompressionOnFilter(unsigned int filterNumber) const
{
    if (filterNumber < compressorNodes.size()){
	CompressorProcessor* compressorProcessor = dynamic_cast<CompressorProcessor*>(compressorNodes[filterNumber]->getProcessor());
	return compressorProcessor->getCompression();
    } else return 0;
}

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
	    compressorProcessor->setAttack(attack);
	    compressorProcessor->setRelease(release);
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
