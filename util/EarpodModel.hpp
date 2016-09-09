#include <vector>
class EarpodModel {
    enum Token {
        VOLUME_UP;
        VOLUME_DOWN;
        NOISE;
    };
    public:
        EarpodModel(std::string fileName);
        ~EarpodModel = default;
        std::vector<Token> read(const std::vector<unsigned char> &data);
        std::vector<Token> readEOF();
}
