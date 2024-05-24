#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <fstream>
#include <chrono>
#include <ctime>

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

// Clase BufferPool
class BufferPool {
private:
    int size; // Tamaño del buffer pool en número de frames
    std::vector<Frame*> frames; // Vector de frames
    std::unordered_map<int, Frame*> page_table; // Tabla de páginas para mapear page_id a frames
    std::list<Frame*> lru_queue; // Lista enlazada para el algoritmo LRU

    // Obtiene un frame libre cuando el frame no tiene una página cargada en el puntero page
    Frame* getFreeFrame() {
        for (auto& frame : frames) {
            if (frame->page == nullptr) {
                return frame;
            }
        }
        return nullptr;
    }

    // evictPage() elige una página para reemplazarla en el buffer pool
    // y la elimina de la tabla de páginas sin no antes escribirla en disco si es necesario
    Frame* evictPage() {
        Frame* victim = chooseVictimFrame();
        if (victim->page->dirty) {
            writePageToDisk(victim->page);
        }
        page_table.erase(victim->page->page_id);
        delete victim->page;
        victim->page = nullptr;
        return victim;
    }

    // chooseVictimFrame() elige la página a reemplazar en el buffer pool
    Frame* chooseVictimFrame() {
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
    void writePageToDisk(Page* page) {
        // Lógica para escribir la página en disco
        std::cout << "Escribiendo página " << page->page_id << " al disco." << std::endl;

        // Ejemplo de escritura en un archivo:
        std::ofstream file("pagina_" + std::to_string(page->page_id) + ".txt");
        if (file.is_open()) {
            file << page->block->data;
            file.close();
        }
        page->dirty = false; // Una vez escrita, la página ya no está sucia
    }

    // btiene la hora actual en formato std::time_t para last_used
    std::time_t getCurrentTime() {
        return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }

public:
    // Constructor de BufferPool
    BufferPool(int pool_size) : size(pool_size) {
        for (int i = 0; i < size; ++i) {
            frames.push_back(new Frame(i));
        }
    }

    ~BufferPool() {
        for (auto& frame : frames) {
            delete frame;
        }
    }

    Frame* pinPage(int page_id, Block* block = nullptr) {
        // compara si la página está en el buffer pool
        if (page_table.find(page_id) != page_table.end()) {
            Frame* frame = page_table[page_id];
            frame->page->pin_count++;
            frame->page->last_used = getCurrentTime();
            std::cout << "Pinned página " << page_id << " en el frame " << frame->frame_id << ". Pin count: " << frame->page->pin_count << std::endl;
            return frame;
        } else {
            return loadPage(page_id, block);
        }
    }

    // remueve el Pin de la página y se puede marcar como sucia si dirty = true
    void unpinPage(int page_id, bool dirty = false) {
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
    Frame* loadPage(int page_id, Block* block) {
        Frame* frame = getFreeFrame();

        // si no hay frames libres, se elige una página para reemplazar
        if (frame == nullptr) {
            frame = evictPage();
        }

        // se designa al frame la nueva página
        frame->page = new Page(page_id, block);
        page_table[page_id] = frame;
        frame->page->pin_count = 1;
        frame->page->last_used = getCurrentTime();
        std::cout << "Se cargó la página " << page_id << " en el frame " << frame->frame_id << ". Pin count: " << frame->page->pin_count << std::endl;
        return frame;
    }
};

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
    Frame* requestPage(int page_id, Block* block = nullptr) {
        return buffer_pool->pinPage(page_id, block);
    }

    // libera una página del buffer pool (unpinPage) determinando si está sucia o no
    void releasePage(int page_id, bool dirty = false) {
        buffer_pool->unpinPage(page_id, dirty);
    }
};

int main() {
    BufferManager buffer_manager(3);

    // Crear bloques de datos
    Block* block1 = new Block("Este es el contenido del bloque 1.");
    Block* block2 = new Block("Este es el contenido del bloque 2.");
    Block* block3 = new Block("Este es el contenido del bloque 3.");

    // Solicitar páginas
    Frame* frame1 = buffer_manager.requestPage(1, block1);
    Frame* frame2 = buffer_manager.requestPage(2, block2);
    Frame* frame3 = buffer_manager.requestPage(3, block3);

    // Liberar páginas
    buffer_manager.releasePage(1, true);  // Página 1 marcada como sucia
    buffer_manager.releasePage(2, false); // Página 2 no marcada como sucia

    // Solicitar otra página para provocar un reemplazo
    Block* block4 = new Block("Este es el contenido del bloque 4.");
    Frame* frame4 = buffer_manager.requestPage(4, block4);

    // Liberar más páginas y hacer más reemplazos
    buffer_manager.releasePage(3, true);  // Página 3 marcada como sucia
    buffer_manager.releasePage(4, false); // Página 4 no marcada como sucia

    Block* block5 = new Block("Este es el contenido del bloque 5.");
    Frame* frame5 = buffer_manager.requestPage(5, block5);

    return 0;
}