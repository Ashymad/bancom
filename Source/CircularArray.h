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
