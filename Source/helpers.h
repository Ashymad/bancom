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

#include "IIRFilterCascadeProcessor.h"

namespace helpers {
    template<typename FloatType>
    ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>>
    designIIRLowpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order);

    template<typename FloatType>
    ReferenceCountedArray<dsp::IIR::Coefficients<FloatType>>
    designIIRHighpassHighOrderLRMethod(FloatType frequency, double sampleRate, int order);

    OwnedArray<IIRFilterCascadeProcessor>
    designLRFilterBank(const Array<float>& frequencies, double sampleRate, int order);

    float applyGainSlopeDecibels(AudioSampleBuffer& buffer, float startGainDecibels,
	    float endGainDecibles, float slopeDecibels, int startIndex=0);
}
