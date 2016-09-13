#pragma once

#include <vector>
#include "kiss_fft130/kiss_fft.h"

class Frontend {
    public:
    Frontend(int sampleRate);
    ~Frontend();
    // Give big endian 16-bit pcm
    std::vector<double> calculate(const std::vector<unsigned char> &samples);
    int getSampleSize() {
        return 2 * frameSize_;
    }
    private:
    const int sampleRate_;
    const int frameSize_{2205}; // In samples
    const int fftOutputSize_;
    const int buckets_{1103};
    const int bucketSize_;
    kiss_fft_cfg cfg_;
    kiss_fft_cpx *fin_;
    kiss_fft_cpx *fout_;
};
