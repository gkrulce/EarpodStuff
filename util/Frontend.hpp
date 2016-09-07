#pragma once

#include <vector>

// TODO forward declare these
#include <fftw3.h>

class Frontend {
    public:
    Frontend(int sampleRate, int frameSize);
    ~Frontend();
    std::vector<double> calculate(const std::vector<short> &samples);
    private:
    const int sampleRate_;
    const int frameSize_;
    const int buckets_{10};
    const int bucketSize_;
    fftw_plan plan_;
    fftw_complex *fft_;
};
