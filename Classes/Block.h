#ifndef BLOCK_H
#define BLOCK_H
#include <string>
#include <fstream>
// Clase Block
class Block {
public:
    std::string pathDisk;
    int blockId;

    // Block(const std::string& path) : pathDisk(path) {
    //     blockId = std::stoi(path.substr(path.find("Block") + 5, path.find(".txt") - path.find("Block") - 5));
    // }
    Block(int _blockId) {
        blockId = _blockId;
        pathDisk = "../Disk/Block" + std::to_string(blockId) + ".txt";
    }

    // Block(std::ifstream& file) {
        // std::string line;
        // while (std::getline(file, line)) {
        //     data += line + "\n";
        // }
    // }
};

#endif //BLOCK_H
