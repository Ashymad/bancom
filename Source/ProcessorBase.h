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

#include "../JuceLibraryCode/JuceHeader.h"

class ProcessorBase  : public AudioProcessor
{
    public:
	ProcessorBase() {}
	ProcessorBase(const BusesProperties& properties) : AudioProcessor{properties} {};
	~ProcessorBase() {}

	void prepareToPlay(double, int) override {}
	void releaseResources() override {}
	void processBlock(AudioSampleBuffer&, MidiBuffer&) override {}
	
	AudioProcessorEditor* createEditor() override { return nullptr; }
	bool hasEditor() const override { return false; }

	const String getName() const override { return {}; }
	bool acceptsMidi() const override { return false; }
	bool producesMidi() const override { return false; }
	bool isMidiEffect() const override { return false; }
	double getTailLengthSeconds() const override { return 0; }

	int getNumPrograms() override { return 0; }
	int getCurrentProgram() override { return 0; }
	void setCurrentProgram(int) override {}
	const String getProgramName(int) override { return {}; }
	void changeProgramName(int, const String&) override {}

	void getStateInformation(MemoryBlock& destData) override {}
	void setStateInformation(const void* data, int sizeInBytes) override {}

    private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorBase)
};
