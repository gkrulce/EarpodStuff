#include "EarpodModel.hpp"
#include <iostream>

using namespace std;

int main() {
    string modelFile = "/Users/gkrulce/Documents/iListen/util/nnSimple.model";
    EarpodModel model(modelFile);
    auto result = model.read(std::vector<unsigned char>(20000, 0));
    model.readEOF();
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
