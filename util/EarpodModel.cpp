#include "EarpodModel.hpp"
#include <fstream>
#include <iostream>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;
typedef EarpodModel EM;
EM::Matrix EM::matMul(const EM::Matrix &lhs, const EM::Matrix &rhs) {
    int lhsR = lhs.size();
    int lhsC = lhs.at(0).size();
    int rhsR = rhs.size();
    int rhsC = rhs.at(0).size();
    assert(lhsC == rhsR);
    EM::Matrix result(lhsR);
    for(int i = 0; i < lhsR; ++i) {
        result.at(i) = vector<double>(rhsC);
        for(int j = 0; j < rhsC; ++j) {
            double mult = 0.0;
            for(int k = 0; k < lhsC; ++k) {
                mult += lhs.at(i).at(k) * rhs.at(k).at(j);
            }
            result.at(i).at(j) = mult;
        }
    }
    return result;
}

EM::Matrix EM::matAdd(const EM::Matrix &lhs, const EM::Matrix &rhs) {
    assert(lhs.size() == rhs.size());
    assert(lhs.at(0).size() == rhs.at(0).size());
    int r = lhs.size();
    int c = lhs.at(0).size();
    Matrix mat(r);
    for(int i = 0; i < r; ++i) {
        mat.at(i) = vector<double>(c);
        for(int j = 0; j < c; ++j) {
            mat[i][j] = lhs.at(i).at(j) + rhs.at(i).at(j);
        }
    }
    return mat;
}

EM::Matrix EM::softMax(const Matrix &mat) {
    assert(mat.size() == 1);
    vector<double> vec = mat.at(0);
    double sum = 0.0;
    for(auto &val: vec) {
        sum += exp(val);
    }
    vector<double> result(vec.size());
    for(int i = 0; i < result.size(); ++i) {
        result.at(i) = exp(vec.at(i))/sum;
    }
    Matrix matRes;
    matRes.push_back(result);
    return matRes;
}

void EM::print(const Matrix &mat) {
    for(int i = 0; i < mat.size(); ++i) {
        for(int j = 0; j < mat.at(i).size(); ++j) {
            cout << mat.at(i).at(j) << ",";
        }
        cout << endl;
    }
}

EM::EarpodModel(std::string fileName) {
    ifstream inFile (fileName);
    string t;
    while(true) {
        string name;
        int r,c;
        inFile >> name >> r >> c;
        if(inFile.eof()) {
            return;
        }
        cout << "Read matrix " << name << " with dimensions " << r << "x" << c << endl;
        getline(inFile, t);
        EM::Matrix mat (r);
        for(int i = 0; i < r; ++i) {
            mat[i] = vector<double>(c);
            string line;
            getline(inFile, line);
            stringstream lineSS(line);
            string tok;
            for(int j = 0; j < c; ++j) {
                getline(lineSS, tok, ',');
                mat[i][j] = atof(tok.c_str());
            }
        }
        mats_[name] = mat;
    }

}
vector<EM::Token> EM::read(const std::vector<unsigned char> &data) {
    // TODO use a frame shifting algorithm instead
    vector<EM::Token> returnedTokens;
    buffer_.insert(buffer_.end(), data.begin(), data.end());
    while(buffer_.size() >= frameSize_*2) {
        auto beginRange = buffer_.begin();
        auto endRange = buffer_.begin() + frameSize_*2;
        vector<unsigned char> toSend(beginRange, endRange);
        buffer_.erase(beginRange, endRange);
        vector<double> featuresVector = frontend_.calculate(toSend);
        EM::Matrix features;
        features.push_back(featuresVector);
        Matrix outputMat = softMax(matAdd(matMul(features, mats_["W"]), mats_["b"]));
        assert(outputMat.size() == 1);
        std::vector<double> output= outputMat.at(0);
        assert(output.size() == 3);
        auto sum = accumulate(output.begin(), output.end(), 0.0);
        assert(fabs(1.0-sum) < .001);
        cout << endl;
        auto max = max_element(output.begin(), output.end());
        if(max == output.begin()) {
            returnedTokens.push_back(EM::Token::VOLUME_UP);
        }else if(max == output.begin() + 1) {
            returnedTokens.push_back(EM::Token::NOISE);
        }else if(max == output.begin() + 2) {
            returnedTokens.push_back(EM::Token::VOLUME_DOWN);
        }else {
            assert(false);
        }
    }
    return returnedTokens;
}
std::vector<EM::Token> EM::readEOF() {
    if(buffer_.size() == 0){
        return std::vector<EM::Token>();
    }
    vector<unsigned char> zeros(frameSize_*2 - buffer_.size(), '\0');
    return read(zeros);
}
