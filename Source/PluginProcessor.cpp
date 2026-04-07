#include "PluginProcessor.h"
#include "PluginEditor.h"

SeraphicGenesisProcessor::SeraphicGenesisProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "Parameters", createParameterLayout())
{
    for (int i = 0; i < numPolyVoices; ++i)
        synth.addVoice (new SeraphicVoice());

    synth.addSound (new SeraphicSound());
}

SeraphicGenesisProcessor::~SeraphicGenesisProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout SeraphicGenesisProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID ("CUTOFF", 1), "Cutoff", 20.0f, 20000.0f, 2000.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID ("RESONANCE", 1), "Resonance", 0.0f, 1.0f, 0.1f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID ("DETUNE", 1), "Detune", 0.0f, 0.1f, 0.01f));
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID ("VITALITY", 1), "Vitality Amount", 0.0f, 1.0f, 0.5f));

    return layout;
}

void SeraphicGenesisProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    vitalityEngine.setSampleRate (sampleRate);
    synth.setCurrentPlaybackSampleRate (sampleRate);

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SeraphicVoice*> (synth.getVoice (i)))
        {
            juce::dsp::ProcessSpec spec;
            spec.sampleRate = sampleRate;
            spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
            spec.numChannels = 1; // Each voice is mono internally
            voice->filter.prepare (spec);

            for (auto& osc : voice->oscillators)
                osc.prepareToPlay (sampleRate, samplesPerBlock);
            
            voice->adsr.setSampleRate (sampleRate);
            juce::ADSR::Parameters adsrParams;
            adsrParams.attack = 0.5f;   // Slower, swelling attack like pure light
            adsrParams.decay = 0.8f;
            adsrParams.sustain = 0.9f;
            adsrParams.release = 2.5f;  // Long, ethereal tail
            voice->adsr.setParameters (adsrParams);
        }
    }
}

void SeraphicGenesisProcessor::releaseResources() {}

bool SeraphicGenesisProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void SeraphicGenesisProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Advance theta and update global Vitality
    theta += 0.00001f * static_cast<float>(buffer.getNumSamples());
    if (theta > std::numbers::pi_v<float> * 2.0f)
        theta -= std::numbers::pi_v<float> * 2.0f;

    updateParameters();

    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    // FINAL SERAPHIC FINISH: Angelic Spectral Shimmer
    // A subtle high-shelf boost derived from pi and phi to add that "pro producer" bow-inducing air.
    const float phi = 1.61803398875f;
    
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* data = buffer.getWritePointer(channel);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float x = data[i];
            
            // 1. Seraphic Purity Limiter
            if (std::abs(x) > 0.8f) {
                x = (x > 0) ? 0.8f + (x - 0.8f) / (1.0f + std::pow(x - 0.8f, 2)) 
                            : -0.8f + (x + 0.8f) / (1.0f + std::pow(x + 0.8f, 2));
            }
            
            // 2. Spectral Air Excitation (very subtle 12kHz+ shelf)
            // This adds the "angelic breath" to the final signal
            static float z1 = 0.0f; 
            float highFreq = x - z1; // Simple high-pass
            z1 = x;
            x += highFreq * 0.02f * (1.0f + 0.5f * std::sin(theta * phi)); 
            
            data[i] = x;
        }
    }
}

void SeraphicGenesisProcessor::updateParameters()
{
    float vitalityAmount = *apvts.getRawParameterValue ("VITALITY");
    float phiMod = vitalityEngine.getPhiModulation (theta) * vitalityAmount;

    float baseCutoff = *apvts.getRawParameterValue ("CUTOFF");
    float baseRes = *apvts.getRawParameterValue ("RESONANCE");
    float baseDetune = *apvts.getRawParameterValue ("DETUNE");

    float modCutoff = baseCutoff * (1.0f + 0.2f * std::sin (phiMod));
    float modRes = baseRes * (1.0f + 0.1f * std::cos (phiMod));
    float modDetune = baseDetune * (1.0f + 0.05f * std::sin (phiMod * 0.5f));
    float stretch = 0.001f * std::abs (std::sin (theta * 0.5f)) * vitalityAmount;

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SeraphicVoice*> (synth.getVoice (i)))
        {
            voice->filter.setCutoff (modCutoff);
            voice->filter.setResonance (modRes);
            voice->detuneAmount = modDetune;
            voice->harmonicStretch = stretch;
        }
    }
}

// SeraphicVoice implementation
SeraphicGenesisProcessor::SeraphicVoice::SeraphicVoice() {}

void SeraphicGenesisProcessor::SeraphicVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    float freq = static_cast<float> (juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber));
    const float phi = 1.61803398875f;

    for (int i = 0; i < numUnisonVoices; ++i)
    {
        float exponent = static_cast<float> (i - numUnisonVoices / 2);
        float detunedFreq = freq * (1.0f + detuneAmount * std::pow (phi, exponent));
        
        // Pristine phi-based stereo spread
        float pan = std::sin(static_cast<float>(i) * phi) * 0.8f;

        // 3D Phase Bloom: Each voice starts at a different golden-ratio offset
        // This creates a blossoming effect rather than a harsh digital click.
        float startingPhase = std::fmod(static_cast<float>(i) * (1.0f / phi), 1.0f);

        oscillators[static_cast<size_t>(i)].setFrequency (detunedFreq);
        oscillators[static_cast<size_t>(i)].setPan (pan);
        oscillators[static_cast<size_t>(i)].setPhase (startingPhase);
        oscillators[static_cast<size_t>(i)].setHarmonicStretch (harmonicStretch);
        oscillators[static_cast<size_t>(i)].updateWavetable();
    }

    adsr.noteOn();
}

void SeraphicGenesisProcessor::SeraphicVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();
}

void SeraphicGenesisProcessor::SeraphicVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!adsr.isActive()) return;

    // We render into a stereo buffer for the voice
    juce::AudioBuffer<float> voiceBuffer (2, numSamples);
    voiceBuffer.clear();
    
    auto* channelDataL = voiceBuffer.getWritePointer (0);
    auto* channelDataR = voiceBuffer.getWritePointer (1);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float mixedL = 0.0f;
        float mixedR = 0.0f;
        
        for (auto& osc : oscillators)
        {
            float oscSample = osc.getNextSample();
            float pan = osc.getPan();
            
            // Constant-power panning
            float panAngle = (pan + 1.0f) * std::numbers::pi_v<float> * 0.25f; 
            float gainL = std::cos(panAngle);
            float gainR = std::sin(panAngle);

            mixedL += oscSample * gainL;
            mixedR += oscSample * gainR;
        }

        // Normalize by sqrt of unison voices to preserve perceived loudness
        float normalize = 1.0f / std::sqrt(static_cast<float>(numUnisonVoices));
        channelDataL[sample] = mixedL * normalize;
        channelDataR[sample] = mixedR * normalize;
    }

    juce::dsp::AudioBlock<float> block (voiceBuffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    filter.process (context);

    // Apply the Seraphic ADSR envelope
    adsr.applyEnvelopeToBuffer (voiceBuffer, 0, numSamples);

    // Add back to the main output buffer
    for (int channel = 0; channel < std::min(voiceBuffer.getNumChannels(), outputBuffer.getNumChannels()); ++channel)
        outputBuffer.addFrom (channel, startSample, voiceBuffer, channel, 0, numSamples);

    if (!adsr.isActive())
        clearCurrentNote();
}

juce::AudioProcessorEditor* SeraphicGenesisProcessor::createEditor()
{
    return new SeraphicGenesisEditor (*this);
}

void SeraphicGenesisProcessor::getStateInformation (juce::MemoryBlock& destData) {}
void SeraphicGenesisProcessor::setStateInformation (const void* data, int sizeInBytes) {}

// This creates the processor
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SeraphicGenesisProcessor();
}
