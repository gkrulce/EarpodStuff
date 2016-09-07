#include "Frontend.hpp"
#include <assert.h>
#include <numeric>
#include <iostream>

using namespace std;

Frontend::Frontend(int sampleRate, int frameSize): sampleRate_(sampleRate), frameSize_(frameSize),
bucketSize_(frameSize_/buckets_) {
    assert(frameSize_ % buckets_ == 0); // Makes the math easier :)
    fft_ = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * frameSize_);
    plan_ = fftw_plan_dft_1d(frameSize_, fft_, fft_, FFTW_FORWARD, FFTW_ESTIMATE);
}

Frontend::~Frontend() {
    fftw_destroy_plan(plan_);
    fftw_free(fft_);
}

vector<double> Frontend::calculate(const vector<short> &samples) {
    assert(sampleRate_ == 44100); // TODO delete me
    assert(frameSize_ == 2048); // Ditto
    assert(samples.size() == frameSize_);
    for(int i = 0; i < frameSize_; ++i) {
        fft_[i][0] = samples.at(i)/32768.0;
        assert(fft_[i][0] < 1.0 && fft_[i][0] > -1.0);
        fft_[i][1] = 0;
    }
    plan_ = fftw_plan_dft_1d(frameSize_, fft_, fft_, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan_);
    vector<double> result(frameSize_);
    for(int i = 0; i < frameSize_; ++i) {
        result[i] = fft_[i][0];
    }

    vector<double> toReturn(buckets_);

    for(int i = 0; i < buckets_; ++i) {
        toReturn.at(i) = accumulate(result.begin() + (i*bucketSize_), result.begin() + (i+1) * bucketSize_, 0.0);
    }
    return toReturn;
}


