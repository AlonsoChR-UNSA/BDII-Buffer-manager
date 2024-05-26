#ifndef BLOCK_H
#define BLOCK_H
#include <string>
#include <fstream>
// Clase Block
class Block {
public:
    std::string data;

    Block(const std::string& data) : data(data) {}

    Block(std::ifstream& file) {
        std::string line;
        while (std::getline(file, line)) {
            data += line + "\n";
        }
    }
};

#endif //BLOCK_H
