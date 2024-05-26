#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <fstream>
#include <chrono>
#include <ctime>
#include "Classes/BufferPool.h"
#include "Classes/Block.h"
#include "Classes/Page.h"
#include "Classes/Frame.h"
#include "Classes/BufferManager.h"


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