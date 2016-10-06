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
        EarpodModel();
        ~EarpodModel() = default;
        std::vector<Token> read(const std::vector<unsigned char> &data);
        std::vector<Token> readEOF();

    private:
    static Matrix matMul(const Matrix &lhs, const Matrix &rhs);
    static Matrix matAdd(const Matrix &lhs, const Matrix &rhs);
    static Matrix softMax(const Matrix &mat);
    static Matrix sigmoid(const Matrix &mat);
    static Matrix readMatrix(int r, int c, float *data);
    static void print(const Matrix &mat);
    Frontend frontend_ {44100};
    const int sampleSize_;
    std::map<std::string, Matrix> mats_;
    std::deque<unsigned char> buffer_;
};
