#include "PluginProcessor.h"
#include "PluginEditor.h"

SeraphicGenesisEditor::SeraphicGenesisEditor (SeraphicGenesisProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setLookAndFeel (&seraphicLookAndFeel);

    addAndMakeVisible (emeraldFrame);
    addAndMakeVisible (emeraldCore);

    // Resizer Selector
    addAndMakeVisible (sizeSelector);
    sizeSelector.addItem ("Small", 1);
    sizeSelector.addItem ("Medium", 2);
    sizeSelector.addItem ("Large", 3);
    sizeSelector.setSelectedId (2, juce::dontSendNotification);
    sizeSelector.onChange = [this] {
        switch (sizeSelector.getSelectedId()) {
            case 1: setUISize(UISize::Small); break;
            case 2: setUISize(UISize::Medium); break;
            case 3: setUISize(UISize::Large); break;
        }
    };

    // Setup Sliders
    auto setupSlider = [this](juce::Slider& slider, const juce::String& paramID, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment, const juce::String& name) {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
        slider.setName(name); // Used by LookAndFeel for coloring
        addAndMakeVisible (slider);
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, paramID, slider);
    };

    setupSlider (cutoffSlider, "CUTOFF", cutoffAttachment, "Cutoff");
    setupSlider (resoSlider, "RESONANCE", resoAttachment, "Resonance");
    setupSlider (detuneSlider, "DETUNE", detuneAttachment, "Phi Detune");
    setupSlider (vitalitySlider, "VITALITY", vitalityAttachment, "Vitality Amount");

    updateLayoutForSize();
    startTimerHz(60); // 60 FPS update rate for smooth shader vitality sync
}

SeraphicGenesisEditor::~SeraphicGenesisEditor() {
    stopTimer();
    setLookAndFeel (nullptr);
}

void SeraphicGenesisEditor::timerCallback()
{
    if (auto* p = processor.apvts.getRawParameterValue("VITALITY")) {
        emeraldCore.setVitality(p->load());
    }
}

void SeraphicGenesisEditor::setUISize (UISize newSize)
{
    currentSize = newSize;
    updateLayoutForSize();
}

void SeraphicGenesisEditor::updateLayoutForSize()
{
    int width = 800;
    switch (currentSize) {
        case UISize::Small: width = 600; break;
        case UISize::Medium: width = 800; break;
        case UISize::Large: width = 1200; break;
    }

    const float phi = 1.61803398875f;
    setSize (width, static_cast<int>(width / phi));
}

void SeraphicGenesisEditor::paint (juce::Graphics& g)
{
    const float phi = 1.61803398875f;
    const float invPhi = 1.0f / phi;

    // 1. Obsidian-black base (Main body background)
    g.fillAll (juce::Colour::fromFloatRGBA (0.05f, 0.05f, 0.05f, 1.0f));

    auto bounds = getLocalBounds().toFloat();
    const float frameThickness = 40.0f;
    auto innerBounds = bounds.reduced (frameThickness);

    // 2. Iridescent gradient background on inner panel
    juce::ColourGradient grad (juce::Colours::purple.withAlpha(0.05f), innerBounds.getBottomLeft(),
                               juce::Colours::gold.withAlpha(0.05f), innerBounds.getTopRight(), true);
    grad.addColour (0.5, juce::Colours::cyan.withAlpha(0.05f));
    g.setGradientFill (grad);
    g.fillRect (innerBounds);

    // 3. Golden Ratio grid intersections relative to INNER panel
    float x_phi = innerBounds.getX() + innerBounds.getWidth() * invPhi;
    float y_phi = innerBounds.getY() + innerBounds.getHeight() * invPhi;

    g.setColour (juce::Colours::cyan.withAlpha (0.05f)); // Subdued grid
    g.drawVerticalLine (static_cast<int>(x_phi), innerBounds.getY(), innerBounds.getBottom());
    g.drawHorizontalLine (static_cast<int>(y_phi), innerBounds.getX(), innerBounds.getRight());

    // 4. Branding
    g.setColour (juce::Colours::platinum);
    g.setFont (juce::Font ("Serif", 30.0f, juce::Font::plain));
    g.drawText ("SERAPHIC GENESIS", innerBounds.getX() + 20, innerBounds.getY() + 20, 400, 50, juce::Justification::left);
    
    g.setFont (juce::Font ("Sans-Serif", 12.0f, juce::Font::plain));
    g.setColour (juce::Colour::fromFloatRGBA (0.0f, 0.8f, 0.4f, 0.8f)); // Emerald text
    g.drawText ("EMERALD TABLET EDITION", innerBounds.getX() + 20, innerBounds.getY() + 60, 400, 20, juce::Justification::left);

    // 5. Knob Labels
    g.setFont (juce::Font ("Sans-Serif", 10.0f, juce::Font::bold));
    g.setColour(juce::Colours::grey);
    
    auto drawLabel = [&](juce::Slider& s, const juce::String& text, bool isGold = false) {
        if (isGold) g.setColour(juce::Colours::gold.withAlpha(0.8f));
        else g.setColour(juce::Colours::grey);
        g.drawText(text, s.getBounds().withY(s.getBounds().getBottom() + 5).withHeight(15), juce::Justification::centred);
    };

    drawLabel(cutoffSlider, "CUTOFF");
    drawLabel(resoSlider, "RESONANCE");
    drawLabel(detuneSlider, "PHI DETUNE", true);
    drawLabel(vitalitySlider, "VITALITY", true);
}

void SeraphicGenesisEditor::resized()
{
    emeraldFrame.setBounds (getLocalBounds());
    
    auto innerBounds = getLocalBounds().reduced (45); 
    sizeSelector.setBounds (innerBounds.getRight() - 120, innerBounds.getY() + 20, 100, 24);

    const float phi = 1.61803398875f;
    const float invPhi = 1.0f / phi;

    float x_phi = innerBounds.getX() + innerBounds.getWidth() * invPhi;
    float y_phi = innerBounds.getY() + innerBounds.getHeight() * invPhi;

    // Position the 3D Emerald Core exactly at the center
    int coreSize = static_cast<int>(innerBounds.getHeight() * 0.45f);
    emeraldCore.setBounds (innerBounds.getCentreX() - coreSize / 2, innerBounds.getCentreY() - coreSize / 2, coreSize, coreSize);

    // Golden grid placement
    int knobSize = innerBounds.getWidth() > 1000 ? 90 : 70; // Scale knobs with UI

    
    // Left side clustered around horizontal Phi line
    int leftColumnX = static_cast<int>(innerBounds.getX() + (x_phi - innerBounds.getX()) * 0.25f) - knobSize / 2;
    int rightColumnX = static_cast<int>(innerBounds.getX() + (x_phi - innerBounds.getX()) * 0.75f) - knobSize / 2;
    
    int topRowY = static_cast<int>(y_phi - knobSize * 1.5f);
    int bottomRowY = static_cast<int>(y_phi + knobSize * 0.5f);

    cutoffSlider.setBounds (leftColumnX, topRowY, knobSize, knobSize);
    resoSlider.setBounds (leftColumnX, bottomRowY, knobSize, knobSize);
    
    detuneSlider.setBounds (rightColumnX, topRowY, knobSize, knobSize);
    vitalitySlider.setBounds (rightColumnX, bottomRowY, knobSize, knobSize);
}
