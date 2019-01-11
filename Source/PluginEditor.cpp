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
    gainSliders(),
    frequencySliders(),
    addCrossoverButton(),
    removeCrossoverButton(),
    applyButton()
{
    int crosses = 1;

    for (int i = 0; i < crosses; ++i){
	addGainSlider();
	addFrequencySlider();
    }
    addGainSlider();
    
    addCrossoverButton.setButtonText("Add");
    addCrossoverButton.addListener(this);
    addAndMakeVisible(addCrossoverButton);
    removeCrossoverButton.setButtonText("Remove");
    removeCrossoverButton.addListener(this);
    addAndMakeVisible(removeCrossoverButton);
    applyButton.setButtonText("Apply");
    applyButton.addListener(this);
    addAndMakeVisible(applyButton);

    processor.setFrequencies(Array<float>(125.0f));

    setSize (400, 300);
}

void BancomAudioProcessorEditor::addGainSlider()
{
    Slider* slider = new Slider();
    slider->setRange(-20.0f, 20.0f);
    slider->setValue(0, dontSendNotification);
    slider->setTextValueSuffix(" dB");
    slider->setPopupDisplayEnabled (true, false, this);
    slider->addListener(this);
    addAndMakeVisible(slider);
    gainSliders.add(slider);
}

void BancomAudioProcessorEditor::addFrequencySlider()
{
    Slider* slider = new Slider();
    slider->setRange(20.0f, 20000.0f, 1);
    slider->setValue(frequencySliders.size() > 0 ? frequencySliders.getLast()->getValue()*2 : 125.0f, dontSendNotification);
    slider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
    slider->setTextValueSuffix(" Hz");
    slider->setPopupDisplayEnabled (true, false, this);
    addAndMakeVisible(slider);
    frequencySliders.add(slider);
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

void BancomAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &addCrossoverButton && gainSliders.size() < 8)
    {
	addGainSlider();
	addFrequencySlider();
	resized();
    }
    else if (button == &removeCrossoverButton && gainSliders.size() > 2)
    {
	gainSliders.removeLast();
	frequencySliders.removeLast();
	resized();
    }
    else if (button == &applyButton)
    {
	Array<float> frequencies = Array<float>();
	for (Slider* slider : frequencySliders){
	    frequencies.add(slider->getValue());
	}
	processor.setFrequencies(frequencies);
	processor.initialiseFilters();
	processor.prepareGraph();
	processor.connectNodes();
    }
}
void BancomAudioProcessorEditor::resized()
{
    int border = 20;
    int sliderSpacing = 30;
    int frequencySliderWidth = 100;
    for (int i = 0; i < gainSliders.size(); ++i){
	gainSliders[i]->setBounds(border + frequencySliderWidth, 20 + i*sliderSpacing, getWidth() - 2*border - frequencySliderWidth, 20);
    }

    for (int i = 0; i < frequencySliders.size(); ++i){
	frequencySliders[i]->setBounds(border, 35 + i*sliderSpacing, frequencySliderWidth - 10, 20);
    }
    addCrossoverButton.setBounds(border, getHeight() - border - 20, 60, 20);
    removeCrossoverButton.setBounds(border + 60, getHeight() - border - 20, 60, 20);
    applyButton.setBounds(border + 120, getHeight() - border - 20, 60, 20);
}
