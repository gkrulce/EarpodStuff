#include "EarpodModel.hpp"
#include <iostream>
#include <iterator>
#include <fstream>

using namespace std;

int main() {
    string modelFile = "/Users/gkrulce/Documents/iListen/util/nnSimple.model";
    EarpodModel model(modelFile);
    std::ifstream testFile("/Users/gkrulce/Desktop/Audio Samples/VolumeDown/Audio-0.pcm", std::ios::binary);
    std::vector<unsigned char> fileContents((std::istreambuf_iterator<char>(testFile)),
                                   std::istreambuf_iterator<char>());
    std::vector<unsigned char> zeros(32000, '\0');
    auto result = model.read(fileContents);
    auto flushed = model.readEOF();
    result.insert(result.end(), flushed.begin(), flushed.end());
    for(auto &val: result) {
       switch(val) {
            case EarpodModel::VOLUME_UP:
                cout << "Volume up" << endl;
                break;
            case EarpodModel::VOLUME_DOWN:
                cout << "Volume down" << endl;
                break;
            case EarpodModel::NOISE:
                cout << "Noise" << endl;
                break;
       }
    }
}
