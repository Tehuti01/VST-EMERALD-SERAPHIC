# Seraphic Genesis: Supreme Quality Verification Suite

This directory contains the mathematical and structural validation tools for the **Seraphic Genesis: Emerald Tablet Edition**. These tests ensure that the plugin lives up to the "Seraphic" standards of perfection, purity, and divine alignment.

## Test Index

### [01] DSP Consistency & Transcendental Accuracy
**File:** `Seraphic_DSP_Consistency_Test_01.cpp`
*   **Wavetable Purity:** Validates 4th-order Hermite interpolation.
*   **Modulation Growth:** Ensures the $\phi$ spiral modulation follows the logarithmic growth formula $r = a \cdot e^{b\theta}$.
*   **Normalization:** Verifies that 64-harmonic wavetables are perfectly normalized to prevent digital clipping.

### [02] System Integrity & Parameter Linkage
**File:** `Seraphic_System_Integrity_Test_02.cpp`
*   **APVTS Mapping:** Confirms that frontend parameter changes (Vitality, Cutoff) correctly influence the backend DSP math.
*   **UI Proportions:** Validates that the window aspect ratio is strictly locked to the Golden Ratio ($\phi \approx 1.618$) across all resolutions.

### [03] Master Multi-Test Suite (Comprehensive)
**File:** `Seraphic_Multi_Test_Suite_03.cpp`
*   **Polyphony Management:** Stress-tests the 8-voice allocation and voice-stealing logic.
*   **Filter Stability:** Ensures the Seraphic Ladder Filter remains stable (no "NaN explosions") even at extreme resonance settings.
*   **Dynamic Resizer:** Validates that Golden Grid intersection points remain mathematically accurate during Small/Medium/Large window resizing.
*   **Spectral Air:** Verifies the initialization of the 64-harmonic "Angelic Shimmer" engine.

## Execution
To run these tests in a local environment:
1. Ensure a C++23 compliant compiler is installed.
2. Link against the JUCE 8 modules (or use the provided `MockJUCE` headers for pure logic validation).
3. Run the compiled binaries to see the Seraphic Result.

---
*"From the first breath of light — sound that evolves forever."*
