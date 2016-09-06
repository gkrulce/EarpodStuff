#include "Frontend.hpp"
#include <assert.h>
#include <complex>
#include <fftw3.h>

Frontend::Frontend(int sampleRate): sampleRate_(sampleRate) {}

std::vector<float> Frontend::calculate(std::vector<short> samples) {
    assert(sampleRate_ == 44100);
    assert(samples.size() == sampleRate_);
    fftw_complex *in, *out;
    return std::vector<float>();
}


