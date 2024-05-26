#ifndef FRAME_H
#define FRAME_H
#include <iostream>

#include "Page.h"

// Clase Frame
class Frame {
public:
    int frame_id;
    Page* page;

    Frame(int id) : frame_id(id), page(nullptr) {}

    ~Frame() {
        delete page;
    }
    void showPage() {
        std::cout << "== Frame Info ==================================\n";
        std::cout << "Frame: " << frame_id << std::endl;
        std::cout << "Page: " << page->page_id << std::endl;
        std::cout << "Dirty: " << page->dirty << std::endl;
        std::cout << "Pin Count: " << page->pin_count << std::endl;
        std::cout << "Last Used: " << std::ctime(&page->last_used);
        std::cout << "Data: \n" << page->data << std::endl;
        std::cout << "================================================\n";
    }
};

#endif //FRAME_H
