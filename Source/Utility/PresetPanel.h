/*
  ==============================================================================

    PresetPanel.h
    Created: 11 Jan 2024 6:09:12pm
    Author:  kylew

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PresetManager.h"

struct PresetPanel : public juce::Component
{
    PresetPanel(PresetManager& pm);

    void resized() override;

private:

    PresetManager& presetManager;

    void configureButton(juce::Button& button, const juce::String& buttonText);
    void loadPresetList();

    juce::TextButton saveButton, deleteButton, previousPresetButton, nextPresetButton;
    juce::ComboBox presetList;

    std::unique_ptr<juce::FileChooser> fileChooser;

};