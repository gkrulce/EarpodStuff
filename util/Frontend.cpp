#include "Frontend.hpp"
#include <assert.h>
#include <complex>

Frontend::Frontend(int sampleRate): sampleRate_(sampleRate) {}

std::vector<float> Frontend::calculate(std::vector<short> samples) {
    assert(sampleRate_ == 44100);
    assert(samples.size() == sampleRate_);
    return std::vector<float>();
}


