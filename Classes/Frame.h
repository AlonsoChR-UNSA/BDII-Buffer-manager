#ifndef FRAME_H
#define FRAME_H
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
};

#endif //FRAME_H
