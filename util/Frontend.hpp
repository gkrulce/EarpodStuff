#pragma once

#include <vector>

// TODO forward declare these
#include <fftw3.h>

class Frontend {
    public:
    Frontend(int sampleRate, int frameSize);
    ~Frontend();
    std::vector<float> calculate(std::vector<short>);
    private:
    int sampleRate_;
    int frameSize_;
    int buckets_{10};
    fftw_plan plan_;
    fftw_complex *fft_;
};
