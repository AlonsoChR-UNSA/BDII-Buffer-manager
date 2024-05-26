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

    int numberBlocks = 20;
    Block* blockList[numberBlocks];
    // Crear bloques de datos
    for (int i = 1; i <= numberBlocks; ++i) {
        blockList[i-1] = new Block(i);
    }
    // Solicitar páginas
    Frame* frame1 = buffer_manager.requestPage(1 );
    Frame* frame2 = buffer_manager.requestPage(2);
    Frame* frame3 = buffer_manager.requestPage(3);

    // Liberar páginas
    buffer_manager.releasePage(1, true);  // Página 1 marcada como sucia
    buffer_manager.releasePage(2, false); // Página 2 no marcada como sucia

    // Solicitar otra página para provocar un reemplazo
    Frame* frame4 = buffer_manager.requestPage(4);

    // Liberar más páginas y hacer más reemplazos
    buffer_manager.releasePage(3, true);  // Página 3 marcada como sucia
    buffer_manager.releasePage(4, false); // Página 4 no marcada como sucia

    Frame* frame5 = buffer_manager.requestPage(5);

    return 0;
}