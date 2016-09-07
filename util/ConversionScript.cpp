#include "Frontend.hpp"
#include <iostream>
#include <assert.h>
#include <dirent.h> //TODO switch to boost
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;


// Input: Directory name holding .pcm files
// Output: vector of audio in byte strings
vector<string> readAudioFiles(const string &directoryName) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (directoryName.c_str())) != NULL) {
        vector<string> byteStrs;
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            string f = ent->d_name;
            if(f.find(".pcm") != string::npos) {
                string absoluteFilePath = directoryName + "/" + f;
                cout << "Reading: " << absoluteFilePath << endl;
                std::ifstream t(absoluteFilePath);
                std::string str((std::istreambuf_iterator<char>(t)),
                                 std::istreambuf_iterator<char>());
                byteStrs.push_back(str);
            }
        }
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
    string str = origStr;
    assert(str.size() % 2 == 0);
    vector<short> shorts;
    for(int i = 0; i < str.size(); i += 2) {
        char lsb = str.at(i);
        char msb = str.at(i+1);
        short s = (short) ((lsb << 8) | msb);
        shorts.push_back(s);
    }
    return frontend.calculate(shorts);
}

void writeToCSVFile(ostream &out, const vector<double> &features, int classId) {
   out << classId;
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

    Frontend frontend(44100, 2048);

    ofstream outFile;
    outFile.open(outCSVFile);

    auto volUp = readAudioFiles(volUpDir);
    for(auto &s: volUp) {
        writeToCSVFile(outFile, extractFeatures(s, frontend), 0);
    }

    auto volDown = readAudioFiles(volDownDir);
    for(auto &s: volDown) {
        writeToCSVFile(outFile, extractFeatures(s, frontend), 1);
    }

    auto noise = readAudioFiles(noiseDir);
    for(auto &s: noise) {
        writeToCSVFile(outFile, extractFeatures(s, frontend), 2);
    }

    outFile.close();
}