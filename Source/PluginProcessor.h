#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "DSP/WavetableOscillator.h"
#include "DSP/VitalityEngine.h"
#include "DSP/SeraphicFilter.h"

class SeraphicGenesisProcessor : public juce::AudioProcessor
{
public:
    SeraphicGenesisProcessor();
    ~SeraphicGenesisProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Seraphic Genesis"; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override {}
    const juce::String getProgramName (int index) override { return {}; }
    void changeProgramName (int index, const juce::String& newName) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

private:
    static constexpr int numUnisonVoices = 8;
    static constexpr int numPolyVoices = 8;

    struct SeraphicVoice : public juce::SynthesiserVoice
    {
        SeraphicVoice();
        bool canPlaySound (juce::SynthesiserSound* sound) override { return true; }
        void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
        void stopNote (float velocity, bool allowTailOff) override;
        void pitchWheelMoved (int newPitchWheelValue) override {}
        void controllerMoved (int controllerNumber, int newControllerValue) override {}
        void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

        std::array<WavetableOscillator, numUnisonVoices> oscillators;
        SeraphicFilter filter;
        juce::ADSR adsr;
        float detuneAmount = 0.01f;
        float harmonicStretch = 0.0f;
    };

    struct SeraphicSound : public juce::SynthesiserSound
    {
        bool canPlayNote (int midiNoteNumber) override { return true; }
        bool canPlayOnChannel (int midiChannel) override { return true; }
    };

    juce::Synthesiser synth;
    VitalityEngine vitalityEngine;
    float theta = 0.0f;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void updateParameters();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SeraphicGenesisProcessor)
};
