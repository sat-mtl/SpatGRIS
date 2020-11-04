/*
 This file is part of SpatGRIS2.

 Developers: Samuel Béland, Olivier Bélanger, Nicolas Masson

 SpatGRIS2 is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 SpatGRIS2 is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with SpatGRIS2.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "macros.h"

DISABLE_WARNINGS
#include <JuceHeader.h>
ENABLE_WARNINGS

#include "MainComponent.h"

//==============================================================================
/* This class implements the desktop window that contains an instance of
   our MainContentComponent class.
*/
class MainWindow final : public juce::DocumentWindow
{
public:
    enum CommandIDs {
        // File menu
        NewPresetID = 1000,
        OpenPresetID = 1001,
        SavePresetID = 1002,
        SaveAsPresetID = 1003,

        OpenSpeakerSetupID = 2000,
        ShowSpeakerEditID = 2003,

        PrefsID = 9998,
        QuitID = 9999,

        // View menu
        Show2DViewID = 3000,
        ShowNumbersID = 3001,
        ShowSpeakersID = 3002,
        ShowTripletsID = 3003,
        ShowSourceLevelID = 3004,
        ShowSpeakerLevelID = 3005,
        ShowSphereID = 3006,
        ColorizeInputsID = 3010,
        ResetInputPosID = 3011,
        ResetMeterClipping = 3012,
        ShowOscLogView = 3100,

        // Help menu
        AboutID = 4000,
        OpenManualID = 4001,
    };
    //==============================================================================
    explicit MainWindow(juce::String const & name);
    //==============================================================================
    MainWindow() = delete;
    ~MainWindow() = default;

    MainWindow(MainWindow const &) = delete;
    MainWindow(MainWindow &&) = delete;

    MainWindow & operator=(MainWindow const &) = delete;
    MainWindow & operator=(MainWindow &&) = delete;
    //==============================================================================
    bool exitWinApp();
    juce::ApplicationCommandManager & getApplicationCommandManager();
    //==============================================================================
    void closeButtonPressed() override;
    //==============================================================================
    static MainWindow * getMainAppWindow();

private:
    //==============================================================================
    std::unique_ptr<MainContentComponent> mcc{};
    juce::ApplicationCommandManager applicationCommandManager{};
    //=============================================================
    JUCE_LEAK_DETECTOR(MainWindow)
};
