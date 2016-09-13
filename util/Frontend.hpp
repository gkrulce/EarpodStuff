#pragma once

#include <vector>
#include "kiss_fft130/kiss_fft.h"

class Frontend {
    public:
    Frontend(int sampleRate, int frameSize);
    ~Frontend();
    // Give big endian 16-bit pcm
    std::vector<double> calculate(const std::vector<unsigned char> &samples);
    private:
    const int sampleRate_;
    const int frameSize_; // In samples
    const int fftOutputSize_;
    const int buckets_{1103};
    const int bucketSize_;
    kiss_fft_cfg cfg_;
    kiss_fft_cpx *fin_;
    kiss_fft_cpx *fout_;
};
