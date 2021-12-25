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

#include "sg_Macros.hpp"

#include <JuceHeader.h>

namespace gris
{
class GrisLookAndFeel;
class MainContentComponent;

//==============================================================================
class AddRemoveSourcesComponent final
    : public juce::Component
    , juce::TextButton::Listener
    , juce::TextEditor::Listener
{
    MainContentComponent & mMainContentComponent;
    juce::Label mLabel{};
    juce::TextEditor mNumberOfSourcesEditor{};
    juce::TextButton mApplyButton{};

public:
    //==============================================================================
    AddRemoveSourcesComponent(int currentNumberOfSources,
                              MainContentComponent & mainContentComponent,
                              GrisLookAndFeel & lookAndFeel);
    ~AddRemoveSourcesComponent() override = default;
    SG_DELETE_COPY_AND_MOVE(AddRemoveSourcesComponent)
    //==============================================================================
    void buttonClicked(juce::Button *) override;
    void resized() override;
    void textEditorReturnKeyPressed(juce::TextEditor & editor) override;
    //==============================================================================
    static int getWidth();
    static int getHeight();

private:
    //==============================================================================
    void applyAndClose() const;
    //==============================================================================
    JUCE_LEAK_DETECTOR(AddRemoveSourcesComponent)
};

//==============================================================================
class AddRemoveSourcesWindow final : public juce::DocumentWindow
{
    MainContentComponent & mMainContentComponent;
    AddRemoveSourcesComponent mComponent;

public:
    //==============================================================================
    AddRemoveSourcesWindow(int currentNumberOfSources,
                           MainContentComponent & mainContentComponent,
                           GrisLookAndFeel & lookAndFeel);
    ~AddRemoveSourcesWindow() override = default;
    SG_DELETE_COPY_AND_MOVE(AddRemoveSourcesWindow)
    //==============================================================================
    void closeButtonPressed() override;

private:
    //==============================================================================
    JUCE_LEAK_DETECTOR(AddRemoveSourcesWindow)
};

} // namespace gris