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

#include "sg_MinSizedComponent.hpp"

namespace gris
{
class GrisLookAndFeel;

//==============================================================================
class TitledComponent final : public MinSizedComponent
{
    static constexpr auto TITLE_HEIGHT = 18;

    juce::String mTitle{};
    MinSizedComponent * mContentComponent;
    GrisLookAndFeel & mLookAndFeel;

public:
    //==============================================================================
    TitledComponent(juce::String title, MinSizedComponent * contentComponent, GrisLookAndFeel & lookAndFeel);
    ~TitledComponent() override = default;
    SG_DELETE_COPY_AND_MOVE(TitledComponent)
    //==============================================================================
    void resized() override;
    void paint(juce::Graphics & g) override;

    [[nodiscard]] int getMinWidth() const noexcept override { return mContentComponent->getMinWidth(); }
    [[nodiscard]] int getMinHeight() const noexcept override
    {
        return mContentComponent->getMinHeight() + TITLE_HEIGHT;
    }

private:
    //==============================================================================
    JUCE_LEAK_DETECTOR(TitledComponent)
};

} // namespace gris