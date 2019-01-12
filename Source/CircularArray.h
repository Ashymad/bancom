/*
  ==============================================================================

    CircularArray.h
    Created: 12 Jan 2019 9:05:37pm
    Author:  shyman

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

template<typename E>
class CircularArray : public Array<E>
{
    public:
	void clear()
	{
	    Array<E>::clear();
	    currentElement = 0;
	}
	void setAndRotate(const E& element)
	{
	    this->setUnchecked(currentElement, element);
	    currentElement = ++currentElement < this->size() ? currentElement : 0;
	}
    private:
	unsigned int currentElement = 0;
};
