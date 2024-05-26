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

    Page(int id, Block* blk) :
    page_id(id),
    dirty(false),
    pin_count(0),
    last_used(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())),
    block(blk) {}

    ~Page() {
        delete block;
    }
};

#endif //PAGE_H
