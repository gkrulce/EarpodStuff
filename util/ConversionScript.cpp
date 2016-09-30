#include "Frontend.hpp"
#include <iostream>
#include <assert.h>
#include <dirent.h> //TODO switch to boost
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;


// Input: Directory name holding .raw files
// Output: vector of audio in byte strings
vector<string> readAudioFiles(const string &directoryName) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (directoryName.c_str())) != NULL) {
        vector<string> byteStrs;
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            string f = ent->d_name;
            if(f.find(".raw") != string::npos) {
                string absoluteFilePath = directoryName + "/" + f;
                std::ifstream t(absoluteFilePath);
                string str;

                t.seekg(0, std::ios::end);   
                str.reserve(t.tellg());
                t.seekg(0, std::ios::beg);

                str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
                byteStrs.push_back(str);
            }
        }
        cout << "Read " << byteStrs.size() << " files from " << directoryName << endl;
        closedir (dir);
        return byteStrs;
    } else {
        /* could not open directory */
        perror ("");
        return vector<string>();
    }
}

// Input: audio file byte string
// Output vector of features
vector<double> extractFeatures(const string &origStr, Frontend &frontend) {
    assert(origStr.size() % 2 == 0);
    vector<unsigned char> hacky(origStr.size());
    for(int i = 0; i < origStr.size(); ++i) {
        hacky.at(i) = origStr.at(i);
    }
    return frontend.calculate(hacky);
}

void writeToCSVFile(ostream &out, const vector<double> &features, const string &className) {
   out << className;
   for(auto &feat: features) {
        out << "," << feat;
   }
   out << endl;
}


int main(int argc, char * argv[]) {
    assert(argc == 5);
    string outCSVFile = argv[1];
    string volUpDir = argv[2];
    string volDownDir = argv[3];
    string noiseDir = argv[4];

    Frontend frontend(44100);

    ofstream outFile;
    outFile.open(outCSVFile);

    auto volUp = readAudioFiles(volUpDir);
    for(auto &s: volUp) {
        writeToCSVFile(outFile, extractFeatures(s, frontend), "VolUp");
    }

    auto volDown = readAudioFiles(volDownDir);
    for(auto &s: volDown) {
        writeToCSVFile(outFile, extractFeatures(s, frontend), "VolDown");
    }

    auto noise = readAudioFiles(noiseDir);
    for(auto &s: noise) {
        writeToCSVFile(outFile, extractFeatures(s, frontend), "Noise");
    }

    outFile.close();
}
