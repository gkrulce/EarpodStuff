#include "Frontend.hpp"
#include <iostream>
#include <math.h>
#include <assert.h>

using namespace std;
int main() {
    const float PI = 3.1415926535;
    const int sampleRate = 44100;
    const float frequency = 30000;
    const float samplesPerCycle = sampleRate/frequency;
    const int frameSize = 2048;
    vector<unsigned char> in(2*frameSize);
    for(int i = 0; i < in.size(); i += 2) {
        short s = (short) (32767 * sin(2*i*PI/samplesPerCycle));
        unsigned char lsb = (unsigned char) s;
        unsigned char msb = (unsigned char) (s >> 8);
        in.at(i) = lsb;
        in.at(i+1) = msb;
    }

    Frontend frontend(sampleRate, frameSize);
    vector<double> out = frontend.calculate(in);
    for(auto &val : out) {
        cout << val << endl;
    }

}
