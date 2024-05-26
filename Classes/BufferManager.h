#ifndef BUFFERPOOLMANAGER_H
#define BUFFERPOOLMANAGER_H

// Clase BufferManager
class BufferManager {
private:
    BufferPool* buffer_pool;

public:
    BufferManager(int buffer_pool_size) {
        buffer_pool = new BufferPool(buffer_pool_size);
    }

    ~BufferManager() {
        delete buffer_pool;
    }

    // solicita una página del buffer pool (pinPage)
    Frame* requestPage(int block_id) {
        return buffer_pool->pinPage(block_id);
    }

    // libera una página del buffer pool (unpinPage) determinando si está sucia o no
    void releasePage(int page_id, bool dirty = false) {
        buffer_pool->unpinPage(page_id, dirty);
    }

    void showFrames() {
        buffer_pool->showFrames();
    }
};

#endif //BUFFERPOOLMANAGER_H
