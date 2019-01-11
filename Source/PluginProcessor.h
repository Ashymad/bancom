/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "helpers.h"
#include "GainProcessor.h"
#include "CompressorProcessor.h"

using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
using Node = AudioProcessorGraph::Node;

//==============================================================================
/**
*/
class BancomAudioProcessor  : public AudioProcessor
{
    public:
	//==============================================================================
	BancomAudioProcessor();
	~BancomAudioProcessor();

	//==============================================================================
	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

	void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

	//==============================================================================
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram (int index) override;
	const String getProgramName (int index) override;
	void changeProgramName (int index, const String& newName) override;

	//=============================================================================
	void initialiseGraph();
	void connectNodes();
	void initialiseFilters(float sampleRate = 0);
	void prepareGraph (double sampleRate = 0, int samplesPerBlock = 0);

	//=============================================================================
	void setGainOnFilter(unsigned int filterNumber, float newGainDecibels);
	void setFrequencies(const Array<float>& frequencies);

	//==============================================================================
	void getStateInformation (MemoryBlock& destData) override;
	void setStateInformation (const void* data, int sizeInBytes) override;

	//=============================================================================
	void reset() override;
    private:
	std::unique_ptr<AudioProcessorGraph> mainProcessor;

	Node::Ptr audioInputNode;
	Node::Ptr audioOutputNode;

	Array<float> frequencies;

	Array<Node::Ptr> filterNodes;
	Array<Node::Ptr> gainNodes;
	Array<Node::Ptr> compressorNodes;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BancomAudioProcessor)
};
