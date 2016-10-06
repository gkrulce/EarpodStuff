#include "EarpodModel.hpp"
#include "nn.hpp"
#include <fstream>
#include <iostream>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;
typedef EarpodModel EM;
EM::Matrix EM::sigmoid(const EM::Matrix &mat) {
    int r = mat.size();
    int c = mat.at(0).size();
    Matrix ret(r);
    for(int i = 0; i < r; ++i) {
        ret[i] = vector<double>(c);
        for(int j = 0; j < c; ++j) {
            double val = 1/(1 + exp(-1 * mat[i][j]));
            ret[i][j] = val;
        }
    }
    return ret;
}
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
    vector<double> result = mat.at(0);
    double max = *max_element(result.begin(), result.end());
    for(auto &val: result) {
        val = exp(val - max);
    }
    double sum = accumulate(result.begin(), result.end(), 0.0);
    for(auto &val: result) {
        val = val/sum;
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

EM::Matrix EM::readMatrix(int r, int c, float *data) {
    EM::Matrix mat(r);
    for(int i = 0; i < r; ++i) {
        mat.at(i) = vector<double>(c);
        for(int j = 0; j < c; ++j) {
            mat.at(i).at(j) = data[i*c + j];
        }
    }
    return mat;
}

EM::EarpodModel() : sampleSize_(frontend_.getSampleSize()) {
    mats_["W_H"] = readMatrix(W_H_ROWS, W_H_COLS, W_H_DATA);
    mats_["W_DH"] = readMatrix(W_DH_ROWS, W_DH_COLS, W_DH_DATA);
    mats_["W_O"] = readMatrix(W_O_ROWS, W_O_COLS, W_O_DATA);
}
vector<EM::Token> EM::read(const std::vector<unsigned char> &data) {
    // TODO use a frame shifting algorithm instead
    vector<EM::Token> returnedTokens;
    buffer_.insert(buffer_.end(), data.begin(), data.end());
    while(buffer_.size() >= sampleSize_) {
        auto beginRange = buffer_.begin();
        auto endRange = buffer_.begin() + sampleSize_;
        vector<unsigned char> toSend(beginRange, endRange);
        buffer_.erase(beginRange, endRange);
        vector<double> featuresVector = frontend_.calculate(toSend);
        EM::Matrix features;
        features.push_back(featuresVector);
        Matrix h = sigmoid(matMul(features, mats_["W_H"]));
        Matrix dh = sigmoid(matMul(h, mats_["W_DH"]));
        Matrix outputMat = softMax(matMul(dh, mats_["W_O"]));
        assert(outputMat.size() == 1);
        std::vector<double> output= outputMat.at(0);
        assert(output.size() == 3);
        auto sum = accumulate(output.begin(), output.end(), 0.0);
        assert(fabs(1.0-sum) < .001);
        auto max = max_element(output.begin(), output.end());
        if(max == output.begin()) {
            returnedTokens.push_back(EM::Token::VOLUME_UP);
        }else if(max == output.begin() + 1) {
            returnedTokens.push_back(EM::Token::VOLUME_DOWN);
        }else if(max == output.begin() + 2) {
            returnedTokens.push_back(EM::Token::NOISE);
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
    vector<unsigned char> zeros(sampleSize_ - buffer_.size(), '\0');
    return read(zeros);
}
