#include <vector>

class Frontend {
    public:
    Frontend(int sampleRate);
    std::vector<float> calculate(std::vector<short>);
    private:
    int sampleRate_;
};
