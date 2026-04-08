#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <vector>

class WavetableOscillator
{
public:
    WavetableOscillator();

    void prepareToPlay (double sampleRate, int samplesPerBlock);

    /**
     * Resets the phase to a specific value (0.0 to 1.0).
     */
    void setPhase (float newPhase) { currentPhase = newPhase * static_cast<float>(tableSize); }

    /**
     * Returns the current phase (0.0 to 1.0).
     */
    float getPhase() const { return currentPhase / static_cast<float>(tableSize); }

    /**
     * Sets the frequency of the oscillator.
     */
    void setFrequency (float frequency);

    /**
     * Sets the harmonic stretch coefficient.
     */
    void setHarmonicStretch (float stretch) { harmonicStretch = stretch; }

    /**
     * Sets the stereo pan position (-1.0 to 1.0).
     */
    void setPan (float newPan) { pan = newPan; }

    /**
     * Rebuilds the wavetable with current harmonic stretching.
     */
    void updateWavetable() { createWavetable(); }

    /**
     * Returns the next sample from the oscillator.
     */
    float getNextSample();

    /**
     * Returns the current pan value.
     */
    float getPan() const { return pan; }

private:
    /**
     * Generates a single cycle of a sine wave with pi-stretching.
     */
    void createWavetable();

    /**
     * 4th-order Hermite interpolation for pristine, artifact-free highs.
     */
    inline float hermiteInterpolate(float y0, float y1, float y2, float y3, float mu) const
    {
        float m2 = mu * mu;
        float a0 = -0.5f * y0 + 1.5f * y1 - 1.5f * y2 + 0.5f * y3;
        float a1 = y0 - 2.5f * y1 + 2.0f * y2 - 0.5f * y3;
        float a2 = -0.5f * y0 + 0.5f * y2;
        float a3 = y1;
        return a0 * mu * m2 + a1 * m2 + a2 * mu + a3;
    }

    std::vector<float> wavetable;
    int tableSize = 4096; // Increased resolution for supreme purity
    float currentPhase = 0.0f;
    float phaseIncrement = 0.0f;
    float harmonicStretch = 0.0f;
    float pan = 0.0f;
    double sampleRate = 44100.0;
};
