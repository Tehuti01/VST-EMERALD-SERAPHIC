#pragma once

#include "PluginProcessor.h"
#include "UI/EmeraldTabletFrame.h"
#include "UI/SeraphicLookAndFeel.h"
#include "UI/EmeraldCoreShader.h"

class SeraphicGenesisEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    enum class UISize { Small, Medium, Large };

    SeraphicGenesisEditor (SeraphicGenesisProcessor&);
    ~SeraphicGenesisEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

    /**
     * Updates the UI to one of the three predefined sizes.
     */
    void setUISize (UISize newSize);

private:
    SeraphicGenesisProcessor& processor;

    SeraphicLookAndFeel seraphicLookAndFeel;
    EmeraldTabletFrame emeraldFrame;
    EmeraldCoreShader emeraldCore;
    juce::ComboBox sizeSelector;

    // Sliders
    juce::Slider cutoffSlider;
    juce::Slider resoSlider;
    juce::Slider detuneSlider;
    juce::Slider vitalitySlider;

    // Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> detuneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vitalityAttachment;

    UISize currentSize = UISize::Medium;

    void updateLayoutForSize();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SeraphicGenesisEditor)
};
