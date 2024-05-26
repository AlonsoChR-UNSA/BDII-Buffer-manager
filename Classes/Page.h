#ifndef PAGE_H
#define PAGE_H
#include <chrono>

#include "Block.h"

// Clase Page
class Page {
public:
    int page_id;
    bool dirty;
    int pin_count;
    std::time_t last_used;
    Block* block;
    std::string data;


    // Page(Block* blk){
    //     page_id = blk->blockId;
    //     dirty = false;
    //     pin_count = 0;
    //     last_used = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    //     block = blk;
    //
    //     std::ifstream file(block->pathDisk);
    //     std::string line;
    //     while(std::getline(file, line)) {
    //         data += line + "\n";
    //     }
    // }

    Page (int BlockId) {
        page_id= BlockId;
        dirty = false;
        pin_count = 0;
        last_used = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        block = new Block(BlockId);
        std::ifstream file(block->pathDisk);
        std::string line;
        while(std::getline(file, line)) {
            data += line + "\n";
        }
    }
    ~Page() {
        delete block;
    }
};

#endif //PAGE_H
