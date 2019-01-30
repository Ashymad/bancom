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
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BancomAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener, public Button::Listener, public Timer
{
    public:
	BancomAudioProcessorEditor (BancomAudioProcessor&);
	~BancomAudioProcessorEditor();

	//==============================================================================
	void paint (Graphics&) override;
	void resized() override;
	void sliderValueChanged (Slider* slider) override;
	void buttonClicked (Button* button) override;
	void addGainSlider();
	void addFrequencySlider();
	void addAttackSlider();
	void addReleaseSlider();
	void addRatioSlider();
	void addThresholdSlider();
	void addLevelMeterGraphics();
	void timerCallback() override;

    private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	BancomAudioProcessor& processor;

	OwnedArray<Image> levelMeterImages;
	OwnedArray<Graphics> levelMeterGraphics;

	OwnedArray<Slider> gainSliders;
	OwnedArray<Slider> frequencySliders;
	OwnedArray<Slider> attackSliders;
	OwnedArray<Slider> releaseSliders;
	OwnedArray<Slider> ratioSliders;
	OwnedArray<Slider> thresholdSliders;

	TextButton addCrossoverButton;
	TextButton removeCrossoverButton;
	TextButton applyButton;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BancomAudioProcessorEditor)
};
