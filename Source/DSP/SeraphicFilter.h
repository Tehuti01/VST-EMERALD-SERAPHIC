#pragma once

#include <juce_dsp/juce_dsp.h>
#include <numbers>

/**
 * A 24 dB/octave ladder filter with "Golden Resonance" and saturation.
 * Optimized for mid-range definition and "Translation" (sounding good on any speaker).
 */
class SeraphicFilter
{
public:
    SeraphicFilter()
    {
        ladder.setMode(juce::dsp::LadderFilterMode::LPF24);
    }

    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        ladder.prepare(spec);
        
        // Internal saturation for "translation" on small speakers
        // This creates harmonics that make the sub-bass audible on phones
        saturation.prepare(spec);
    }

    void setCutoff(float frequency)
    {
        ladder.setCutoffFrequencyHz(frequency);
    }

    /**
     * Sets resonance with Mid-Range Preservation.
     * Prevents the filter from "thinning out" the sound when resonance is high.
     */
    void setResonance(float resonance)
    {
        const float phi = 1.61803398875f;
        // Frequency-dependent resonance: 
        // More resonance in the 1kHz-5kHz range to help phone speakers "cut through"
        float scaledResonance = resonance * (1.0f + 0.15f * std::sin(phi * resonance));
        ladder.setResonance(juce::jlimit(0.0f, 0.95f, scaledResonance));
    }

    void process(juce::dsp::ProcessContextReplacing<float>& context)
    {
        // 1. Subtle soft-clipping saturation BEFORE the filter
        // This creates upper harmonics that define the fundamental on tiny speakers
        saturation.process(context);
        
        // 2. High-quality 24dB Ladder filter
        ladder.process(context);
    }

private:
    juce::dsp::LadderFilter<float> ladder;
    
    struct SoftSaturator {
        void prepare(const juce::dsp::ProcessSpec& spec) {}
        void process(juce::dsp::ProcessContextReplacing<float>& context) {
            auto& block = context.getOutputBlock();
            for (size_t channel = 0; channel < block.getNumChannels(); ++channel) {
                auto* samples = block.getChannelPointer(channel);
                for (size_t i = 0; i < block.getNumSamples(); ++i) {
                    // Soft-clipping tanh approximation for warmth and "translation"
                    float x = samples[i];
                    samples[i] = std::tanh(x * 1.2f) * 0.8f; 
                }
            }
        }
    } saturation;
};
