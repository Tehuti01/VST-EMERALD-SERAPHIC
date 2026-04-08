#include "WavetableOscillator.h"
#include <numbers>

WavetableOscillator::WavetableOscillator()
{
    createWavetable();
}

void WavetableOscillator::prepareToPlay (double sr, int samplesPerBlock)
{
    sampleRate = sr;
    currentPhase = 0.0f;
}

void WavetableOscillator::reset()
{
    currentPhase = 0.0f;
}

void WavetableOscillator::setFrequency (float frequency)
{
    phaseIncrement = frequency * static_cast<float>(tableSize) / static_cast<float>(sampleRate);
}

float WavetableOscillator::getNextSample()
{
    int index1 = static_cast<int>(currentPhase);
    int index0 = (index1 - 1 + tableSize) % tableSize;
    int index2 = (index1 + 1) % tableSize;
    int index3 = (index1 + 2) % tableSize;

    float frac = currentPhase - static_cast<float>(index1);

    float y0 = wavetable[static_cast<size_t>(index0)];
    float y1 = wavetable[static_cast<size_t>(index1)];
    float y2 = wavetable[static_cast<size_t>(index2)];
    float y3 = wavetable[static_cast<size_t>(index3)];

    // 4th-order Hermite Interpolation for pristine, angelic high frequencies
    float sample = hermiteInterpolate(y0, y1, y2, y3, frac);

    currentPhase += phaseIncrement;

    while (currentPhase >= static_cast<float>(tableSize))
        currentPhase -= static_cast<float>(tableSize);

    return sample;
}

void WavetableOscillator::createWavetable()
{
    wavetable.assign (static_cast<size_t>(tableSize), 0.0f);

    const int numHarmonics = 64; // Extreme resolution for "Ultra Supreme" quality
    const float pi = std::numbers::pi_v<float>;
    const float phi = 1.61803398875f;

    for (int n = 1; n <= numHarmonics; ++n)
    {
        float stretch = 1.0f + harmonicStretch * std::pow (pi, static_cast<float>(n - 1));
        
        // Seraphic "Pure Light" Harmonic Profile
        // Fundamental is massive, then a "Golden Slope" for the rest
        float amplitude = 1.0f / std::pow(static_cast<float>(n), phi * 0.5f); 
        
        // Angelic Shimmer: Add a high-frequency excitation at pi-derived ratios
        if (n > 16) {
            amplitude *= 1.2f; // Slight boost to the "air" frequencies for that bow-inducing shimmer
        }

        for (int i = 0; i < tableSize; ++i)
        {
            auto angle = 2.0f * pi * static_cast<float>(i) / static_cast<float>(tableSize);
            wavetable[static_cast<size_t>(i)] += amplitude * std::sin (angle * static_cast<float>(n) * stretch);
        }
    }

    // Normalize for maximum dynamic range
    float maxAbs = 0.0f;
    for (auto v : wavetable) maxAbs = std::max(maxAbs, std::abs(v));
    if (maxAbs > 0.0f) {
        for (auto& v : wavetable) v /= maxAbs;
    }
}

void WavetableOscillator::reset()
{
    // Instead of zero, we use a phi-based starting phase to create a "3D Phase Bloom"
    // This will be set by the Voice during startNote
}
