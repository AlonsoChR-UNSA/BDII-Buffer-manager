#include "BufferPool.h"

#include <iomanip>

// Obtiene un frame libre cuando el frame no tiene una página cargada en el puntero page
Frame* BufferPool::getFreeFrame() {
    for (auto& frame : frames) {
        if (frame->page == nullptr) {
            return frame;
        }
    }
    return nullptr;
}

// evictPage() elige una página para reemplazarla en el buffer pool
// y la elimina de la tabla de páginas sin no antes escribirla en disco si es necesario
Frame* BufferPool::evictPage() {
    Frame* victim = chooseVictimFrame();
    if(victim == nullptr) {
        throw std::runtime_error("El BufferPool está lleno y no se puede cargar más páginas.");
    }
    if (victim->page->dirty) {
        writePageToDisk(victim->page);
    }

    page_table.erase(victim->page->page_id);
    delete victim->page;
    victim->page = nullptr;
    return victim;
}

// chooseVictimFrame() elige la página a reemplazar en el buffer pool
Frame* BufferPool::chooseVictimFrame() {
    // usando una función lambda para ordenar la lista de frames por last_used
    lru_queue.sort([](Frame* a, Frame* b) { return a->page->last_used < b->page->last_used; });
    for (auto it = lru_queue.begin(); it != lru_queue.end(); ++it) {
        if ((*it)->page->pin_count == 0) {
            Frame* victim = *it;
            lru_queue.erase(it);
            return victim;
        }
    }
    return nullptr;
}

// actualiza la página en disco con la nueva información
void BufferPool::writePageToDisk(Page* page) {
    // Lógica para escribir la página en disco
    std::cout << "Escribiendo página " << page->page_id << " al disco." << std::endl;

    // Ejemplo de escritura en un archivo:
    std::ofstream file("pagina_" + std::to_string(page->page_id) + ".txt");
    if (file.is_open()) {
        file << page->block->pathDisk;
        file.close();
    }
    page->dirty = false; // Una vez escrita, la página ya no está sucia
}

// obtiene la hora actual en formato std::time_t para last_used
std::time_t BufferPool::getCurrentTime() {
    return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}


// CONSTRUCTOR


// Constructor de BufferPool
BufferPool::BufferPool(int pool_size) : size(pool_size) {
    for (int i = 0; i < size; ++i) {
        frames.push_back(new Frame(i+1000));
    }
}

// Destructor de BufferPool
BufferPool::~BufferPool() {
    for (auto& frame : frames) {
        delete frame;
    }
}

// pinPage() busca una página y la fija, si no está en el buffer pool, la carga
Frame* BufferPool::pinPage(int block_id) {
    // compara si la página está en el buffer pool
    if (page_table.find(block_id) != page_table.end()) {
        Frame* frame = page_table[block_id];
        frame->page->pin_count++;
        frame->page->last_used = getCurrentTime();
        std::cout << "Pinned página " << block_id << " en el frame " << frame->frame_id << ". Pin count: " << frame->page->pin_count << std::endl;
        return frame;
    } else {
        return loadPage(block_id);
    }
}

// remueve el Pin de la página y se puede marcar como sucia si dirty = true
void BufferPool::unpinPage(int page_id, bool dirty ) {
    if (page_table.find(page_id) != page_table.end()) {
        Frame* frame = page_table[page_id];
        frame->page->pin_count--;
        frame->page->dirty = dirty;
        std::cout << "Unpinned página " << page_id << " en el frame " << frame->frame_id << ". Pin count: " << frame->page->pin_count << ". Dirty: " << frame->page->dirty << std::endl;
        if (frame->page->pin_count == 0) {
            lru_queue.push_back(frame);
        }
    }
}

// carga una página en el buffer pool
// con un page_id y un bloque de datos
Frame* BufferPool::loadPage(int block_id) {
    Frame* frame = getFreeFrame();

    // si no hay frames libres, se elige una página para reemplazar
    if (frame == nullptr) {
        frame = evictPage();
    }
    // se designa al frame la nueva página
    frame->page = new Page(block_id);
    page_table[block_id] = frame;
    frame->page->pin_count = 1;
    frame->page->last_used = getCurrentTime();
    std::cout << "Se cargó la página " << block_id << " en el frame " << frame->frame_id << ". Pin count: " << frame->page->pin_count << std::endl;
    return frame;
}

void BufferPool::showFrames() {
    // imprime cada frame y el id de la página que contiene
    std::cout << "############################################\n";
    for (auto& frame : frames) {
        std::cout << "#  Frame: " ;
        std::cout << std::setw(5)<< frame->frame_id << "   ";
        std::cout << "Page: " ;
        if (frame->page != nullptr) {
            std::cout << std::setw(4)<< frame->page->page_id << std::endl;
        } else {
            std::cout << std::setw(4)<< 'X'<< std::endl;
        }
    }

    std::cout << "############################################\n";
}

