/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "helpers.h"
#include "GainProcessor.h"

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

	//=============================================================================
	void setGainOnFilter(unsigned int filterNumber, float newGainDecibels);

	//==============================================================================
	void getStateInformation (MemoryBlock& destData) override;
	void setStateInformation (const void* data, int sizeInBytes) override;

    private:
	std::unique_ptr<AudioProcessorGraph> mainProcessor;

	Node::Ptr audioInputNode;
	Node::Ptr audioOutputNode;

	OwnedArray<IIRFilterCascadeProcessor> filterBank;
	Array<Node::Ptr> filterNodes;
	Array<Node::Ptr> gainNodes;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BancomAudioProcessor)
};
