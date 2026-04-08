#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include "../Source/DSP/VitalityEngine.h"
#include "../Source/DSP/WavetableOscillator.h"

/**
 * SERAPHIC TEST NR. 1: DSP Consistency & Transcendental Accuracy
 * Tested: VitalityEngine (Phi-Modulation), WavetableOscillator (Normalization & Purity)
 */

void testPhiModulation() {
    std::cout << "Testing VitalityEngine Phi-Modulation..." << std::endl;
    VitalityEngine engine;
    const float phi = 1.61803398875f;
    
    // Test that modulation follows the logarithmic spiral growth
    float mod0 = engine.getPhiModulation(0.0f);
    float modPi = engine.getPhiModulation(std::numbers::pi_v<float>);
    
    std::cout << "  - Mod at 0: " << mod0 << std::endl;
    std::cout << "  - Mod at PI: " << modPi << std::endl;
    
    assert(mod0 == 1.0f); // e^0 = 1
    assert(modPi > 1.0f);
    std::cout << "  [PASS] Phi-Modulation sequence is valid." << std::endl;
}

void testWavetablePurity() {
    std::cout << "Testing WavetableOscillator Purity..." << std::endl;
    WavetableOscillator osc;
    osc.prepareToPlay(44100.0, 512);
    osc.setHarmonicStretch(0.01f);
    osc.updateWavetable();
    
    // Check for NaNs and normalization
    bool hasNaN = false;
    float maxVal = 0.0f;
    
    for(int i = 0; i < 4096; ++i) {
        float s = osc.getNextSample();
        if (std::isnan(s)) hasNaN = true;
        if (std::abs(s) > maxVal) maxVal = std::abs(s);
    }
    
    std::cout << "  - Max Amplitude: " << maxVal << std::endl;
    assert(!hasNaN);
    assert(maxVal <= 1.01f); // Allowing small float epsilon for Hermite
    std::cout << "  [PASS] Wavetable is pure and normalized." << std::endl;
}

int main() {
    std::cout << "--- STARTING SERAPHIC TEST NR. 1 ---" << std::endl;
    
    try {
        testPhiModulation();
        testWavetablePurity();
        std::cout << "--- ALL TESTS PASSED: THE SYSTEM IS SERAPHIC ---" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "!!! TEST FAILED: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
