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

BancomAudioProcessorEditor::BancomAudioProcessorEditor(BancomAudioProcessor& p) :
    AudioProcessorEditor{&p},
    processor{p}
{
    MemoryBlock data;
    processor.getStateInformation(data);
    MemoryInputStream stream{data, false};
    ValueTree processorTree{ValueTree::readFromStream(stream)};

    for (int i = 0; i < processorTree.getNumChildren(); ++i){
	addGainSlider();
	addRatioSlider();
	addAttackSlider();
	addReleaseSlider();
	addThresholdSlider();
	addLevelMeterGraphics();

	ValueTree bTree = processorTree.getChild(i);
	if (i > 0)
	{
	    addFrequencySlider();
	    frequencySliders.getLast()->setValue(bTree.getProperty("frequency", dontSendNotification));
	}

	gainSliders.getLast()->setValue(bTree.getProperty("gain"), dontSendNotification);
	ratioSliders.getLast()->setValue(bTree.getProperty("ratio"), dontSendNotification);
	attackSliders.getLast()->setValue(static_cast<float>(bTree.getProperty("attack"))*1000, dontSendNotification);
	releaseSliders.getLast()->setValue(static_cast<float>(bTree.getProperty("release"))*1000, dontSendNotification);
	thresholdSliders.getLast()->setValue(bTree.getProperty("threshold"), dontSendNotification);
    }
    
    addCrossoverButton.setButtonText("Add");
    addCrossoverButton.addListener(this);
    addAndMakeVisible(addCrossoverButton);
    removeCrossoverButton.setButtonText("Remove");
    removeCrossoverButton.addListener(this);
    addAndMakeVisible(removeCrossoverButton);
    applyButton.setButtonText("Apply");
    applyButton.addListener(this);
    addAndMakeVisible(applyButton);

    startTimerHz(30);

    setSize (1530, 300);
}

void BancomAudioProcessorEditor::addLevelMeterGraphics()
{
    Image* image = new Image(Image::PixelFormat::RGB, 300, 20, true);
    levelMeterImages.add(image);
    Graphics* graphics = new Graphics(*image);
    levelMeterGraphics.add(graphics);
}

void BancomAudioProcessorEditor::addGainSlider()
{
    Slider* slider = new Slider();
    slider->setRange(0.0f, 40.0f, 0.1f);
    slider->setValue(0.0f, dontSendNotification);
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
void BancomAudioProcessorEditor::addAttackSlider()
{
    Slider* slider = new Slider();
    slider->setRange(0.0f, 1000.0f, 1);
    slider->setValue(3.0f);
    slider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
    slider->setTextValueSuffix(" ms");
    slider->setPopupDisplayEnabled (true, false, this);
    slider->addListener(this);
    addAndMakeVisible(slider);
    attackSliders.add(slider);
}
void BancomAudioProcessorEditor::addReleaseSlider()
{
    Slider* slider = new Slider();
    slider->setRange(0.0f, 5000.0f, 10);
    slider->setValue(100.0f);
    slider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
    slider->setTextValueSuffix(" ms");
    slider->setPopupDisplayEnabled (true, false, this);
    slider->addListener(this);
    addAndMakeVisible(slider);
    releaseSliders.add(slider);
}
void BancomAudioProcessorEditor::addRatioSlider()
{
    Slider* slider = new Slider();
    slider->setRange(1.0f, 90.0f, 0.1f);
    slider->setSkewFactor(0.5f);
    slider->setValue(1.0f, dontSendNotification);
    slider->setTextValueSuffix(":1");
    slider->setPopupDisplayEnabled (true, false, this);
    slider->addListener(this);
    addAndMakeVisible(slider);
    ratioSliders.add(slider);
}
void BancomAudioProcessorEditor::addThresholdSlider()
{
    Slider* slider = new Slider();
    slider->setRange(-90.0f, 0.0f, 0.1f);
    slider->setSkewFactor(2.0f);
    slider->setValue(0.0f, dontSendNotification);
    slider->setTextValueSuffix(" dB");
    slider->setPopupDisplayEnabled (true, false, this);
    slider->addListener(this);
    addAndMakeVisible(slider);
    thresholdSliders.add(slider);
}

BancomAudioProcessorEditor::~BancomAudioProcessorEditor()
{
}

//==============================================================================
void BancomAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    for (int i = 0; i < levelMeterImages.size(); ++i)
    {
	g.drawImageAt(*levelMeterImages[i], 20 + 3*100 + 900, 20 + i*30);
    }
    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void BancomAudioProcessorEditor::timerCallback()
{
    int maxSize = 300;
    float range = 90;

    for (int i = 0; i < levelMeterImages.size(); ++i)
    {
	float rms = processor.getRMSOnFilter(i);
	rms = rms < -range ? -range : rms;
	int rmsSize = roundToInt(maxSize*(range + rms)/range);
	float comp = processor.getCompressionOnFilter(i);
	int compSize = -roundToInt(maxSize*comp/range);
	int gainSize = roundToInt(maxSize*gainSliders[i]->getValue()/range);
	levelMeterGraphics[i]->fillAll(Colour(0,0,0));
	levelMeterGraphics[i]->setColour(Colour(0, 255, 0));
	levelMeterGraphics[i]->fillRect(0, 0, gainSize, 20);
	levelMeterGraphics[i]->setColour(Colour(255, 255, 0));
	levelMeterGraphics[i]->fillRect(gainSize, 0, rmsSize, 20);
	levelMeterGraphics[i]->setColour(Colour(255, 0, 0));
	levelMeterGraphics[i]->fillRect(gainSize+rmsSize - compSize, 0, compSize, 20);
	levelMeterGraphics[i]->setColour(Colour(0, 0, 255));
	levelMeterGraphics[i]->fillRect(gainSize+roundToInt(maxSize*(range + thresholdSliders[i]->getValue())/range), 0, 2, 20);
    }
    repaint();
}

void BancomAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    for(int i = 0; i < gainSliders.size(); ++i)
    {
	if(slider == gainSliders[i])
	{
	    processor.setGainOnFilter(i, slider->getValue());
	}
	else if (slider == attackSliders[i])
	{
	    processor.setAttackOnFilter(i, slider->getValue()/1000);
	}
	else if (slider == releaseSliders[i])
	{
	    processor.setReleaseOnFilter(i, slider->getValue()/1000);
	}
	else if (slider == thresholdSliders[i])
	{
	    processor.setThresholdOnFilter(i, slider->getValue());
	}
	else if (slider == ratioSliders[i])
	{
	    processor.setRatioOnFilter(i, slider->getValue());
	}
    }
}

void BancomAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &addCrossoverButton && gainSliders.size() < 8)
    {
	addFrequencySlider();
	addGainSlider();
	addRatioSlider();
	addAttackSlider();
	addReleaseSlider();
	addThresholdSlider();
	addLevelMeterGraphics();
	resized();
    }
    else if (button == &removeCrossoverButton && gainSliders.size() > 2)
    {
	gainSliders.removeLast();
	attackSliders.removeLast();
	releaseSliders.removeLast();
	thresholdSliders.removeLast();
	ratioSliders.removeLast();
	frequencySliders.removeLast();
	levelMeterImages.removeLast();
	levelMeterGraphics.removeLast();
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
	thresholdSliders[i]->setBounds(border + frequencySliderWidth, 20 + i*sliderSpacing, 300, 20);
	ratioSliders[i]->setBounds(border + frequencySliderWidth + 300, 20 + i*sliderSpacing, 300, 20);
	attackSliders[i]->setBounds(border + frequencySliderWidth + 600, 20 + i*sliderSpacing, frequencySliderWidth, 20);
	releaseSliders[i]->setBounds(border + 2*frequencySliderWidth + 600, 20 + i*sliderSpacing, frequencySliderWidth, 20);
	gainSliders[i]->setBounds(border + 3*frequencySliderWidth + 600, 20 + i*sliderSpacing, 300, 20);
    }

    for (int i = 0; i < frequencySliders.size(); ++i){
	frequencySliders[i]->setBounds(border, 35 + i*sliderSpacing, frequencySliderWidth - 10, 20);
    }
    addCrossoverButton.setBounds(border, getHeight() - border - 20, 60, 20);
    removeCrossoverButton.setBounds(border + 60, getHeight() - border - 20, 60, 20);
    applyButton.setBounds(border + 120, getHeight() - border - 20, 60, 20);
}
