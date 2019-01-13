/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
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

	OwnedArray<Slider> gainSliders;
	OwnedArray<Slider> frequencySliders;
	OwnedArray<Slider> attackSliders;
	OwnedArray<Slider> releaseSliders;
	OwnedArray<Slider> ratioSliders;
	OwnedArray<Slider> thresholdSliders;

	OwnedArray<Image> levelMeterImages;
	OwnedArray<Graphics> levelMeterGraphics;

	TextButton addCrossoverButton;
	TextButton removeCrossoverButton;
	TextButton applyButton;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BancomAudioProcessorEditor)
};
