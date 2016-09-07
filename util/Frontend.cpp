#include "Frontend.hpp"
#include <assert.h>

Frontend::Frontend(int sampleRate, int frameSize): sampleRate_(sampleRate), frameSize_(frameSize) {
    fft_ = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * frameSize_);
    plan_ = fftw_plan_dft_1d(frameSize_, fft_, fft_, FFTW_FORWARD, FFTW_ESTIMATE);
}

Frontend::~Frontend() = default;

std::vector<float> Frontend::calculate(std::vector<short> samples) {
    assert(sampleRate_ == 44100);
    assert(samples.size() == sampleRate_);
    return std::vector<float>();
}


