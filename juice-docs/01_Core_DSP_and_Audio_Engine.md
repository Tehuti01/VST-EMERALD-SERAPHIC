# Seraphic Framework: Core DSP & Audio Engine
**The Akashic Blueprint of the Seraphic Audio Architecture**

This document details the first 17 fundamental JUCE classes used to manifest the audio engine, outlining their purpose, necessity, and divine mathematical application.

### 1. `juce::AudioProcessor`
*   **What it does:** The absolute master brain of the plugin. It dictates how the DAW communicates with our code.
*   **Why it's impossible without it:** DAWs (Ableton, FL Studio) speak different languages (VST3, AU). Without this class, we would have to write hundreds of thousands of lines of low-level C++ specifically for Apple, Microsoft, Steinberg, and CLAP architectures.
*   **Seraphic Concept:** The "Omnipresent Will." It ensures the Seraphic Genesis functions identically in every host environment.

### 2. `juce::AudioBuffer<float>`
*   **What it does:** A pre-allocated block of computer memory that holds the raw audio wavelengths as floating-point numbers (-1.0 to 1.0).
*   **Why it's impossible without it:** Audio is processed in "blocks" (e.g., 512 samples at a time). Dynamically allocating memory during playback causes instant audio dropouts. `AudioBuffer` guarantees a lock-free, zero-allocation memory space.
*   **Seraphic Concept:** The "Blood of the Engine." It holds the mathematically perfect Hermite-interpolated waveforms as they travel to the speakers.

### 3. `juce::MidiBuffer`
*   **What it does:** Stores timestamped MIDI events (Note On, Note Off, Pitch Bend).
*   **Why it's impossible without it:** MIDI events arrive asynchronously from the human player. This buffer strictly orders them by sample-accurate timestamps so a note triggers at the exact microsecond the producer hits the key.
*   **Seraphic Concept:** The "Human Will." It captures the physical intent and translates it into mathematical triggers for the Phi-Eternal Engine.

### 4. `juce::AudioProcessorValueTreeState` (APVTS)
*   **What it does:** The thread-safe parameter manager.
*   **Why it's impossible without it:** GUI threads and Audio threads operate at vastly different speeds. If the UI modifies a variable exactly as the DSP reads it, a race condition occurs, causing a crash. APVTS uses atomic pointers to safely pass data.
*   **Seraphic Concept:** The "Nervous System." It translates the Golden Ratio visual UI movements into instantaneous DSP recalculations.

### 5. `juce::dsp::ProcessSpec`
*   **What it does:** Passes the exact Sample Rate (e.g., 44.1kHz) and Block Size to all DSP modules.
*   **Why it's impossible without it:** Filters and Oscillators rely on time. A filter calculated for 44.1kHz will sound completely broken at 96kHz. `ProcessSpec` normalizes the math.
*   **Seraphic Math:** Ensures the $\phi$-modulation evolves at the exact same physical speed regardless of the studio's sample rate.

### 6. `juce::Synthesiser`
*   **What it does:** The voice manager and polyphony allocator.
*   **Why it's impossible without it:** Managing multiple notes requires complex "voice stealing" logic (determining which note to kill when the producer plays a 9th note on an 8-voice synth). 
*   **Seraphic Concept:** The "Choir Director." It orchestrates the 8 Golden-panned unison voices flawlessly.

### 7. `juce::SynthesiserVoice`
*   **What it does:** Represents a single note of polyphony.
*   **Why it's impossible without it:** It encapsulates an entire synthesizer (oscillators, filters, envelopes) into a single object that can be dynamically spawned and killed.
*   **Seraphic Concept:** A single "Seraph." Each voice calculates its own 3D Phase Bloom offset.

### 8. `juce::dsp::LadderFilter`
*   **What it does:** A zero-delay feedback (ZDF) 24dB low-pass filter.
*   **Why it's impossible without it:** Simulating analog circuitry in digital space usually introduces a 1-sample delay in the feedback loop, ruining the resonance. ZDF solves this using implicit differential equations.
*   **Seraphic Math:** We intercept this filter to inject **Frequency-Dependent Resonance** and $\phi$-tuned self-oscillation.

### 9. `juce::dsp::StateVariableTPTFilter`
*   **What it does:** Topology Preserving Transform filter (Multi-mode).
*   **Why it's impossible without it:** Standard biquad filters "blow up" (output NaN) if the cutoff frequency changes too fast. TPT filters mathematically guarantee absolute stability during extreme modulation.
*   **Seraphic Concept:** Used in the "Angelic Shimmer" stage to smoothly apply the 12kHz+ high-shelf excitation.

### 10. `juce::ADSR`
*   **What it does:** Attack, Decay, Sustain, Release envelope generator.
*   **Why it's impossible without it:** Notes cannot start and stop instantly; they would cause massive digital clicks.
*   **Seraphic Math:** Tuned to a 0.5s Attack and 2.5s Release to create an exponential "Bloom" rather than a linear fade.

### 11. `juce::dsp::Oversampling`
*   **What it does:** Runs internal DSP at 2x, 4x, or 8x the project sample rate, then filters it back down.
*   **Why it's impossible without it:** High-harmonic synthesis creates frequencies above human hearing (Nyquist limit). If not oversampled, these frequencies "fold back" down into audible frequencies, creating ugly metallic noise (Aliasing).
*   **Seraphic Concept:** The "Purity Enforcer." Guarantees that the 64-harmonic Wavetable remains mathematically pristine.

### 12. `juce::dsp::FFT` (Fast Fourier Transform)
*   **What it does:** Converts audio from the Time Domain (waveforms) into the Frequency Domain (spectrogram).
*   **Why it's impossible without it:** $\pi$-derived harmonic stretching cannot be done by just stretching a wave. You must break the wave into its individual sine waves, shift them, and rebuild them.
*   **Seraphic Math:** The foundation of the Spectral Air engine.

### 13. `juce::dsp::WindowingFunction`
*   **What it does:** Applies a mathematical curve (Hann, Hamming) to chunks of audio before FFT processing.
*   **Why it's impossible without it:** FFT assumes a wave loops infinitely. If you cut a chunk that doesn't perfectly loop, you get "Spectral Leakage." Windowing fades the edges of the chunk to zero.
*   **Seraphic Math:** Ensures the spectral stretching remains hyper-clean.

### 14. `juce::dsp::Convolution`
*   **What it does:** Multiplies an audio signal by an Impulse Response (IR).
*   **Why it's impossible without it:** Recreating the exact reverberation of a cathedral requires convoluting millions of samples per second.
*   **Seraphic Concept:** Reserved for the upcoming *Seraphic Elysium* plugin to simulate physical platinum acoustic spaces.

### 15. `juce::dsp::DelayLine`
*   **What it does:** Holds past audio samples and reads them back with fractional interpolation.
*   **Why it's impossible without it:** Used for physical modeling (comb filters) or chorus effects. Without fractional delay, pitch modulation sounds stepped and broken.
*   **Seraphic Concept:** The engine behind the $\phi$-modulated unison detune.

### 16. `juce::AudioPlayHead`
*   **What it does:** Extracts tempo, BPM, and time signature from the DAW.
*   **Why it's impossible without it:** To sync the Vitality Engine to the track's tempo, the plugin must query the host environment seamlessly.
*   **Seraphic Math:** Syncs the Logarithmic Spiral ($\theta$) to exact musical bars.

### 17. `juce::ScopedNoDenormals`
*   **What it does:** Flushes microscopic CPU numbers to zero.
*   **Why it's impossible without it:** When an audio tail fades to near-zero (e.g., 0.0000000000001), the CPU struggles to calculate it, causing massive CPU spikes (Denormalization).
*   **Seraphic Concept:** The "Void Cleanser." Keeps the DSP extremely fast even during the 2.5-second ethereal release tails.
