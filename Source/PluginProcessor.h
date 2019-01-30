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

#pragma once

#include "helpers.h"
#include "GainProcessor.h"
#include "CompressorProcessor.h"

class BancomAudioProcessor  : public ProcessorBase
{
    public:
	using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
	using Node = AudioProcessorGraph::Node;

	BancomAudioProcessor();
	~BancomAudioProcessor();

	const String getName() const override;

	void prepareToPlay (double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

	void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

	bool hasEditor() const override;
	AudioProcessorEditor* createEditor() override;

	void initialiseGraph();
	void initialiseFilters(float sampleRate = 0);
	void prepareGraph (double sampleRate = 0, int samplesPerBlock = 0);
	void connectNodes();

	void setGainOnFilter(unsigned int filterNumber, float newGainDecibels);
	void setAttackOnFilter(unsigned int filterNumber, float newAttack);
	void setRatioOnFilter(unsigned int filterNumber, float newRatio);
	void setThresholdOnFilter(unsigned int filterNumber, float newThreshold);
	void setReleaseOnFilter(unsigned int filterNumber, float newRelease);
	void setFrequencies(const Array<float>& frequencies);

	float getRMSOnFilter(unsigned int filterNumber) const;
	float getCompressionOnFilter(unsigned int filterNumber) const;

	void getStateInformation (MemoryBlock& destData) override;
	void setStateInformation (const void* data, int sizeInBytes) override;

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
