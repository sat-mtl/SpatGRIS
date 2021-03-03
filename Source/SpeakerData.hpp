/*
 This file is part of SpatGRIS.

 Developers: Samuel B�land, Olivier B�langer, Nicolas Masson

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

#include "StrongTypes.hpp"

//==============================================================================
struct SpeakerData {
    speaker_id_t id{};
    float x{};
    float y{};
    float z{};

    float azimuth{};
    float zenith{};
    float radius{};

    float gain{ 1.0f };

    bool hpActive = false;
    double b1{};
    double b2{};
    double b3{};
    double b4{};
    double ha0{};
    double ha1{};
    double ha2{};

    bool isMuted = false;
    bool isSolo = false;

    output_patch_t outputPatch{};

    bool directOut = false;
};
