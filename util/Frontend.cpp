#include "Frontend.hpp"
#include <assert.h>
#include <numeric>
#include <iostream>
#include <cmath>

using namespace std;

Frontend::Frontend(int sampleRate): sampleRate_(sampleRate),
fftOutputSize_(frameSize_/2+1), bucketSize_(fftOutputSize_/buckets_) {
    assert(fftOutputSize_ % buckets_ == 0); // Makes the math easier :)
    cfg_ = kiss_fft_alloc( frameSize_, 0,0,0 );
    fin_ = (kiss_fft_cpx *) malloc(sizeof(kiss_fft_cpx) * frameSize_);
    fout_ = (kiss_fft_cpx *) malloc(sizeof(kiss_fft_cpx) * frameSize_);
}

Frontend::~Frontend() {
    free(fout_);
    free(fin_);
    free(cfg_);
}

vector<double> Frontend::calculate(const vector<unsigned char> &samples) {
    assert(sampleRate_ == 44100); // TODO delete me
    assert(samples.size() == getSampleSize());
    for(int i = 0; i < frameSize_; ++i) {
        char lsb = samples.at(2*i);
        char msb = samples.at(2*i+1);
        double d = ((double) ((lsb << 8) | msb))/32768.0;
        fin_[i].r = d;
        fin_[i].i = 0;
    }
    kiss_fft(cfg_, fin_, fout_);
    vector<double> realOutput(fftOutputSize_);
    vector<double> complexOutput(fftOutputSize_);
    for(int i = 0; i < fftOutputSize_; ++i) {
        realOutput.at(i) = fabs(fout_[i].r);
        complexOutput.at(i) = fabs(fout_[i].i);
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


