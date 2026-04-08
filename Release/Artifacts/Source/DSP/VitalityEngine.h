#pragma once

#include <cmath>
#include <numbers>

class VitalityEngine
{
public:
    VitalityEngine() = default;

    void setSampleRate (double sr) { sampleRate = sr; }

    /**
     * Calculates the modulation value based on the Golden Ratio (phi) spiral.
     * r = a * e^(b * theta)
     */
    float getPhiModulation (float theta) const
    {
        const float phi = 1.61803398875f;
        const float b = std::log (phi) / (std::numbers::pi_v<float> / 2.0f);
        return std::exp (b * theta);
    }

    /**
     * Calculates the pi-derived harmonic stretching.
     * f_n' = f_0 * n * (1 + S * pi^(n-1))
     */
    float getPiStretching (float fundamental, int harmonicIndex, float stretchCoefficient) const
    {
        if (harmonicIndex == 1) return fundamental;
        return fundamental * harmonicIndex * (1.0f + stretchCoefficient * std::pow (std::numbers::pi_v<float>, harmonicIndex - 1));
    }

private:
    double sampleRate = 44100.0;
};
