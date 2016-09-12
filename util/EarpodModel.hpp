#include "Frontend.hpp"
#include <vector>
#include <map>
#include <string>
#include <deque>

class EarpodModel {
    typedef std::vector<std::vector<double> > Matrix;
    public:
        enum Token {
            VOLUME_UP,
            VOLUME_DOWN,
            NOISE
        };
        EarpodModel(std::string fileName);
        ~EarpodModel() = default;
        std::vector<Token> read(const std::vector<unsigned char> &data);
        std::vector<Token> readEOF();

    private:
    static Matrix matMul(const Matrix &lhs, const Matrix &rhs);
    static Matrix matAdd(const Matrix &lhs, const Matrix &rhs);
    static Matrix softMax(const Matrix &mat);
    static void print(const Matrix &mat);
    const int frameSize_ = 2048;
    std::map<std::string, Matrix> mats_;
    Frontend frontend_ {44100, frameSize_};
    std::deque<unsigned char> buffer_;
};
