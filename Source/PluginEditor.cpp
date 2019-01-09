/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BancomAudioProcessorEditor::BancomAudioProcessorEditor (BancomAudioProcessor& p) :
    AudioProcessorEditor (&p),
    processor (p),
    gainSliders (OwnedArray<Slider>())
{
    for (int i = 0; i < 3; ++i){
	Slider* slider = new Slider();
	slider->setRange(-20.0f, 20.0f);
	slider->setValue(0, dontSendNotification);
	slider->setTextValueSuffix(" dB");
	slider->setPopupDisplayEnabled (true, false, this);
	slider->addListener(this);
	addAndMakeVisible(slider);
	gainSliders.add(slider);
    }

    setSize (400, 300);
}

BancomAudioProcessorEditor::~BancomAudioProcessorEditor()
{
}

//==============================================================================
void BancomAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void BancomAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    for(int i = 0; i < gainSliders.size(); ++i)
    {
	if(gainSliders[i] == slider)
	{
	    processor.setGainOnFilter(i, slider->getValue());
	}
    }
}

void BancomAudioProcessorEditor::resized()
{
    int border = 20;
    int sliderSpacing = 30;
    for (int i = 0; i < gainSliders.size(); ++i){
	gainSliders[i]->setBounds(border, 20 + i*sliderSpacing, getWidth() - border, 20);
    }
}
