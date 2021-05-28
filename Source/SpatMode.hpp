/*
 This file is part of SpatGRIS.

 Developers: Samuel Béland, Olivier Bélanger, Nicolas Masson

 SpatGRIS is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 SpatGRIS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with SpatGRIS.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <JuceHeader.h>

#include "lib/tl/optional.hpp"

enum class SpatMode { vbap = 0, lbap };
enum class StereoMode { hrtf, stereo };
//==============================================================================
extern juce::StringArray const SPAT_MODE_STRINGS;
extern juce::StringArray const SPAT_MODE_TOOLTIPS;
extern juce::StringArray const STEREO_MODE_STRINGS;
extern juce::StringArray const STEREO_MODE_TOOLTIPS;
//==============================================================================
juce::String spatModeToString(SpatMode mode);
tl::optional<SpatMode> stringToSpatMode(juce::String const & string);
juce::String stereoModeToString(StereoMode mode);
tl::optional<StereoMode> stringToStereoMode(juce::String const & string);
