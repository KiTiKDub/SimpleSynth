/*
  ==============================================================================

    PresetPanel.cpp
    Created: 11 Jan 2024 6:09:12pm
    Author:  kylew

  ==============================================================================
*/

#include "PresetPanel.h"

PresetPanel::PresetPanel(PresetManager& pm)
    : presetManager(pm)
{
    configureButton(saveButton, "Save");
    configureButton(deleteButton, "Delete");
    configureButton(previousPresetButton, "Previous");
    configureButton(nextPresetButton, "Next");

    presetList.setTextWhenNothingSelected("Init Preset");
    presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    addAndMakeVisible(presetList);
    loadPresetList();

    saveButton.onClick = [this]()
        {
            fileChooser = std::make_unique<juce::FileChooser>(
                "Please enter the name of the preset to save",
                PresetManager::defaultDirectory,
                "*." + PresetManager::extension
            );
            fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser& chooser)
                {
                    const auto resultFile = chooser.getResult();
                    presetManager.savePreset(resultFile.getFileNameWithoutExtension());
                    loadPresetList();
                });
        };

    deleteButton.onClick = [this]()
        {
            presetManager.deletePreset(presetManager.getCurrentPreset());
            loadPresetList();
        };

    previousPresetButton.onClick = [this]()
        {
            const auto index = presetManager.loadPreviousPreset();
            presetList.setSelectedItemIndex(index, juce::dontSendNotification);
        };

    nextPresetButton.onClick = [this]()
        {
            const auto index = presetManager.loadNextPreset();
            presetList.setSelectedItemIndex(index, juce::dontSendNotification);
        };

    presetList.onChange = [this]()
        {
            presetManager.loadPreset(presetList.getItemText(presetList.getSelectedItemIndex()));           
        };
}

void PresetPanel::resized()
{
    auto bounds = getLocalBounds();

    presetList.setBounds(bounds);
}

void PresetPanel::configureButton(juce::Button& button, const juce::String& buttonText)
{
    button.setButtonText(buttonText);
    button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    addAndMakeVisible(button);
}

void PresetPanel::loadPresetList()
{
    presetList.clear(juce::dontSendNotification);
    const auto allPresets = presetManager.getAllPresets();
    const auto currentPreset = presetManager.getCurrentPreset();
    presetList.addItemList(allPresets, 1);
    presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);
}
