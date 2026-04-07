#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
 * The Seraphic LookAndFeel.
 * Responsible for rendering hyper-realistic 3D materials like machined titanium,
 * brushed platinum, and liquid gold indicators.
 */
class SeraphicLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SeraphicLookAndFeel();
    ~SeraphicLookAndFeel() override = default;

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, const float rotaryStartAngle,
                           const float rotaryEndAngle, juce::Slider& slider) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SeraphicLookAndFeel)
};
