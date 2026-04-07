#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

namespace juce {
    typedef double uint32;
    typedef std::string String;

    struct AudioBuffer {
        AudioBuffer(int channels, int samples) : numSamples(samples), numChannels(channels) {
            data.resize(channels, std::vector<float>(samples, 0.0f));
        }
        int getNumSamples() const { return numSamples; }
        int getNumChannels() const { return numChannels; }
        float* getWritePointer(int channel) { return data[channel].data(); }
        void clear() { for(auto& c : data) std::fill(c.begin(), c.end(), 0.0f); }
        std::vector<std::vector<float>> data;
        int numSamples, numChannels;
    };

    struct jlimit {
        static float limit(float min, float max, float val) {
            return std::max(min, std::min(max, val));
        }
    };
}
