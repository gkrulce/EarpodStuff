#include "Frontend.hpp"
#include <iostream>
#include <math.h>
#include <assert.h>

using namespace std;
int main() {
    const float PI = 3.1415926535;
    const int sampleRate = 44100;
    const float frequency = 10000;
    const float samplesPerCycle = sampleRate/frequency;
    const int frameSize = 100000;
    vector<short> in(frameSize);
    for(int i = 0; i < in.size(); ++i) {
        in[i] = (short) (32767 * sin(2*i*PI/samplesPerCycle));
    }

    Frontend frontend(sampleRate, frameSize);
    vector<float> out = frontend.calculate(in);
    for(auto &val : out) {
        cout << val;
    }

    assert(count_if(out.begin(), out.end(), [](float val){return val != 0;}) == 1);
}
