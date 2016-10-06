#include "EarpodModel.hpp"
#include <assert.h>
#include <iostream>
#include <iterator>
#include <fstream>

using namespace std;

int main(int argc, char ** argv) {
    assert(argc == 2);
    EarpodModel model;
    std::ifstream testFile(argv[1], std::ios::binary);
    std::vector<unsigned char> fileContents((std::istreambuf_iterator<char>(testFile)),
                                   std::istreambuf_iterator<char>());
    std::vector<unsigned char> zeros(32000, '\1');
    auto result = model.read(fileContents);
    //auto result = model.read(zeros);
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
