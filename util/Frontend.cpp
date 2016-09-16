#include "Frontend.hpp"
#include <assert.h>
#include <numeric>
#include <iostream>
#include <cmath>

using namespace std;

Frontend::Frontend(int sampleRate): sampleRate_(sampleRate) {
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
        unsigned char lsb = samples.at(2*i);
        unsigned char msb = samples.at(2*i+1);
        short s = ((lsb << 8) | msb);
        fin_[i].r = s;
        fin_[i].i = 0;
    }
    kiss_fft(cfg_, fin_, fout_);
    vector<double> realOutput(fftOutputSize_);
    for(int i = 0; i < fftOutputSize_; ++i) {
        realOutput.at(i) = fout_[i].r;
    }
    return realOutput;
}


