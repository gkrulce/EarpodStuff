#include "Frontend.hpp"
#include <assert.h>
#include <numeric>
#include <iostream>
#include <cmath>

using namespace std;

Frontend::Frontend(int sampleRate, int frameSize): sampleRate_(sampleRate), frameSize_(frameSize),
fftOutputSize_(frameSize_/2+1), bucketSize_(fftOutputSize_/buckets_) {
    assert(fftOutputSize_ % buckets_ == 0); // Makes the math easier :)
    in_ = fftw_alloc_real(frameSize_);
    out_ = fftw_alloc_complex(fftOutputSize_);
    plan_ = fftw_plan_dft_r2c_1d(frameSize_, in_, out_, FFTW_ESTIMATE);
}

Frontend::~Frontend() {
    fftw_destroy_plan(plan_);
    fftw_free(out_);
    fftw_free(in_);
}

vector<double> Frontend::calculate(const vector<short> &samples) {
    assert(sampleRate_ == 44100); // TODO delete me
    assert(frameSize_ == 2048); // Ditto
    assert(samples.size() == frameSize_);
    for(int i = 0; i < frameSize_; ++i) {
        in_[i] = samples.at(i)/32768.0;
        assert(in_[i] < 1.0 && in_[i] > -1.0);
    }
    plan_ = fftw_plan_dft_r2c_1d(frameSize_, in_, out_, FFTW_ESTIMATE);
    fftw_execute(plan_);
    vector<double> realOutput(fftOutputSize_);
    vector<double> complexOutput(fftOutputSize_);
    for(int i = 0; i < fftOutputSize_; ++i) {
        realOutput.at(i) = fabs(out_[i][0]);
        complexOutput.at(i) = fabs(out_[i][1]);
    }

    // TODO Maybe capture complex output as well?
    std::vector<double> realBuckets(buckets_);
    for(int i = 0; i < buckets_; ++i) {
        realBuckets.at(i) = accumulate(realOutput.begin() + (i*bucketSize_), realOutput.begin() + (i+1) * bucketSize_, 0.0);
    }
    std::vector<double> complexBuckets(buckets_);
    for(int i = 0; i < buckets_; ++i) {
       complexBuckets.at(i) = accumulate(complexOutput.begin() + (i*bucketSize_), complexOutput.begin() + (i+1) * bucketSize_, 0.0);
    }

    std::vector<double> toReturn;
    toReturn.insert(toReturn.end(), realBuckets.begin(), realBuckets.end());
    toReturn.insert(toReturn.end(), complexBuckets.begin(), complexBuckets.end());
    return toReturn;
}


