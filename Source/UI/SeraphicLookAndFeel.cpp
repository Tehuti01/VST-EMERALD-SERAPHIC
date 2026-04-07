#include "SeraphicLookAndFeel.h"
#include <numbers>

SeraphicLookAndFeel::SeraphicLookAndFeel()
{
}

void SeraphicLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                            float sliderPos, const float rotaryStartAngle,
                                            const float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat();
    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toX = bounds.getCentreX();
    auto toY = bounds.getCentreY();
    
    // Reduce slightly to allow for shadows
    radius -= 6.0f;

    // Is this a "Vitality/Gold" knob or standard Cyan?
    bool isGold = slider.getName().containsIgnoreCase("Vitality") || slider.getName().containsIgnoreCase("Detune");
    juce::Colour glowColor = isGold ? juce::Colours::gold : juce::Colours::cyan;

    // --- 1. Drop Shadow (Hyper-realistic casting onto obsidian) ---
    juce::Path knobPath;
    knobPath.addEllipse (toX - radius, toY - radius, radius * 2.0f, radius * 2.0f);
    juce::DropShadow shadow (juce::Colours::black.withAlpha (0.9f), 12, { 0, 4 });
    shadow.drawForPath (g, knobPath);

    // --- 2. Base Titanium Body (Machined Outer Edge) ---
    // Conical/Radial gradient to simulate anisotropic metal
    juce::Colour baseMetal = juce::Colour::fromFloatRGBA (0.15f, 0.15f, 0.16f, 1.0f);
    juce::Colour brightMetal = juce::Colour::fromFloatRGBA (0.4f, 0.4f, 0.42f, 1.0f);
    
    juce::ColourGradient titaniumGrad (brightMetal, toX, toY - radius, baseMetal, toX, toY + radius, false);
    titaniumGrad.addColour(0.5, juce::Colour::fromFloatRGBA(0.08f, 0.08f, 0.09f, 1.0f));
    
    g.setGradientFill (titaniumGrad);
    g.fillPath (knobPath);

    // Outer Edge Specular Highlight (The crisp rim)
    g.setColour(juce::Colours::white.withAlpha(0.3f));
    g.drawEllipse(toX - radius, toY - radius, radius * 2.0f, radius * 2.0f, 1.0f);

    // --- 3. Inner Machined Groove (Stepping inwards) ---
    float innerRadius = radius * 0.75f;
    juce::Path innerGroove;
    innerGroove.addEllipse (toX - innerRadius, toY - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);
    
    // Inverted gradient for depth
    juce::ColourGradient innerGrad (baseMetal.darker(0.8f), toX, toY - innerRadius, brightMetal.darker(0.3f), toX, toY + innerRadius, false);
    g.setGradientFill (innerGrad);
    g.fillPath (innerGroove);
    
    // Inner groove shadow/highlight
    g.setColour(juce::Colours::black.withAlpha(0.6f));
    g.drawEllipse(toX - innerRadius, toY - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f, 1.5f);

    // --- 4. The Glowing Seraphic Ring ---
    float glowRadius = radius * 0.55f;
    g.setColour (glowColor.withAlpha (0.15f));
    g.fillEllipse (toX - glowRadius, toY - glowRadius, glowRadius * 2.0f, glowRadius * 2.0f); // Soft outer glow spread
    
    g.setColour (glowColor.withAlpha (0.4f));
    g.drawEllipse (toX - glowRadius, toY - glowRadius, glowRadius * 2.0f, glowRadius * 2.0f, 2.5f); // Crisp inner ring

    // Add a tiny pure white core to the glow ring
    g.setColour(juce::Colours::white.withAlpha(0.6f));
    g.drawEllipse (toX - glowRadius, toY - glowRadius, glowRadius * 2.0f, glowRadius * 2.0f, 0.5f);

    // --- 5. Liquid Indicator ---
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    juce::Path pointer;
    float pointerLength = radius * 0.85f;
    float pointerThickness = 3.5f;
    
    pointer.addRoundedRectangle (-pointerThickness * 0.5f, -pointerLength, pointerThickness, pointerLength - innerRadius * 0.3f, pointerThickness * 0.5f);
    pointer.applyTransform (juce::AffineTransform::rotation (angle).translated (toX, toY));

    // Liquid gold/cyan indicator material
    juce::Colour indicatorColor = isGold ? juce::Colours::gold : juce::Colours::cyan;
    juce::ColourGradient indicatorGrad (juce::Colours::white, pointer.getBounds().getTopLeft(), indicatorColor, pointer.getBounds().getBottomRight(), false);
    
    // Indicator Shadow
    juce::DropShadow indShadow (juce::Colours::black.withAlpha (0.8f), 3, { 0, 1 });
    indShadow.drawForPath(g, pointer);

    // Fill Indicator
    g.setGradientFill (indicatorGrad);
    g.fillPath (pointer);
    
    // Indicator Glow
    g.setColour(indicatorColor.withAlpha(0.5f));
    g.strokePath(pointer, juce::PathStrokeType(1.5f));

    // --- 6. Golden Ratio (Phi) Engraved Scale Markings ---
    g.setColour (juce::Colours::white.withAlpha(0.15f));
    int numMarks = 13; // Fibonacci number
    for (int i = 0; i < numMarks; ++i)
    {
        float markPos = static_cast<float>(i) / static_cast<float>(numMarks - 1);
        float markAngle = rotaryStartAngle + markPos * (rotaryEndAngle - rotaryStartAngle);
        
        juce::Path mark;
        float markLength = 3.0f;
        float markRadius = radius + 3.0f;
        
        // Every golden ratio step (roughly every 5th mark) is slightly longer/brighter
        bool isPhiMark = (i % 5 == 0); 
        if (isPhiMark) {
            markLength = 5.0f;
            g.setColour (juce::Colours::white.withAlpha(0.3f));
        } else {
            g.setColour (juce::Colours::white.withAlpha(0.15f));
        }

        mark.addLineSegment (juce::Line<float> (0.0f, -markRadius, 0.0f, -markRadius - markLength), isPhiMark ? 1.5f : 1.0f);
        mark.applyTransform (juce::AffineTransform::rotation (markAngle).translated (toX, toY));
        g.strokePath (mark, juce::PathStrokeType (isPhiMark ? 1.5f : 1.0f));
    }
}
