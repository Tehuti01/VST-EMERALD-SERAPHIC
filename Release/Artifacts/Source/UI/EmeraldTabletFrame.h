#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
 * A hyper-realistic frame component inspired by the Emerald Tablet.
 * Upgraded to "Supreme Seraphic Quality" with simulated subsurface scattering,
 * complex bevels, and environmental reflections.
 */
class EmeraldTabletFrame : public juce::Component
{
public:
    EmeraldTabletFrame() = default;

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        const float thickness = 40.0f; // Visual "4-6cm" thickness

        // 1. Environmental Backdrop (Deep space void behind the tablet)
        g.fillAll (juce::Colours::black);

        // 2. Base Emerald Material with Subsurface Scattering
        juce::Colour deepEmerald = juce::Colour::fromFloatRGBA (0.01f, 0.10f, 0.05f, 1.0f);
        juce::Colour glowingEmerald = juce::Colour::fromFloatRGBA (0.05f, 0.35f, 0.15f, 1.0f);
        
        // Radial gradient to simulate internal glow (subsurface scattering)
        juce::ColourGradient sssGrad (glowingEmerald, bounds.getCentreX(), bounds.getCentreY() - 100.0f,
                                      deepEmerald, bounds.getBottomLeft().getX(), bounds.getBottomLeft().getY(), true);
        g.setGradientFill (sssGrad);
        g.fillRoundedRectangle (bounds, 16.0f);

        // 3. Complex Multi-Stage Bevel (Simulating carved, polished crystal)
        juce::Path outerBevel, innerBevel;
        outerBevel.addRoundedRectangle (bounds.reduced (1.0f), 16.0f);
        outerBevel.addRoundedRectangle (bounds.reduced (15.0f), 12.0f);
        outerBevel.setUsingNonZeroWinding (false);

        innerBevel.addRoundedRectangle (bounds.reduced (15.0f), 12.0f);
        innerBevel.addRoundedRectangle (bounds.reduced (thickness), 8.0f);
        innerBevel.setUsingNonZeroWinding (false);

        // Outer Bevel (Catching environmental light)
        juce::ColourGradient outerGrad (juce::Colours::white.withAlpha(0.2f), bounds.getTopLeft(),
                                        juce::Colours::black.withAlpha(0.6f), bounds.getBottomRight(), false);
        g.setGradientFill (outerGrad);
        g.fillPath (outerBevel);

        // Inner Bevel (Refracting into the core)
        juce::ColourGradient innerGrad (deepEmerald.darker(0.8f), bounds.getTopLeft(),
                                        glowingEmerald.brighter(0.5f), bounds.getBottomRight(), false);
        innerGrad.addColour (0.5, juce::Colour::fromFloatRGBA (0.0f, 0.5f, 0.2f, 0.8f)); // Caustic hot-spot
        g.setGradientFill (innerGrad);
        g.fillPath (innerBevel);

        // 4. Golden Edge Highlights (The "Seraphic" touch)
        g.setColour (juce::Colours::gold.withAlpha (0.4f));
        g.drawRoundedRectangle (bounds.reduced (15.0f), 12.0f, 1.5f); // Ridge line
        
        g.setColour (juce::Colours::white.withAlpha (0.6f));
        g.drawRoundedRectangle (bounds.reduced (thickness), 8.0f, 2.0f); // Inner crisp edge

        // 5. Inner Core Drop Shadow (Depth into the obsidian plugin face)
        juce::Path corePath;
        corePath.addRoundedRectangle(bounds.reduced(thickness), 8.0f);
        juce::DropShadow ds (juce::Colours::black.withAlpha (0.95f), 30, { 0, 10 });
        ds.drawForPath (g, corePath);
        
        // Inner Caustic Glow (Light bleeding from the core onto the tablet inner edge)
        juce::DropShadow caustic (juce::Colour::fromFloatRGBA(0.0f, 1.0f, 0.5f, 0.3f), 15, { 0, 0 });
        caustic.drawForPath (g, corePath);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EmeraldTabletFrame)
};
