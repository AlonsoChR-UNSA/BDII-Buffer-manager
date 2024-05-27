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

    // // Solicitar páginas
    // Frame* frame1 = buffer_manager.requestPage(1 );
    // Frame* frame2 = buffer_manager.requestPage(2);
    // Frame* frame3 = buffer_manager.requestPage(3);
    // std::cout << "------------------------------------------>primero\n";
    // buffer_manager.showFrames();
    //
    // // Liberar páginas
    // buffer_manager.releasePage(1, true);  // Página 1 marcada como sucia
    // buffer_manager.releasePage(2, false); // Página 2 no marcada como sucia
    // std::cout << "------------------------------------------>primero\n";
    // buffer_manager.showFrames();
    //
    // // Solicitar otra página para provocar un reemplazo
    // Frame* frame4 = buffer_manager.requestPage(4);
    // std::cout << "------------------------------------------>primero\n";
    // buffer_manager.showFrames();
    //
    // // Liberar más páginas y hacer más reemplazos
    // buffer_manager.releasePage(3, true);  // Página 3 marcada como sucia
    // buffer_manager.releasePage(4, false); // Página 4 no marcada como sucia
    // std::cout << "------------------------------------------>primero\n";
    // buffer_manager.showFrames();
    //
    // Frame* frame5 = buffer_manager.requestPage(5);
    // std::cout << "------------------------------------------>primero\n";
    // buffer_manager.showFrames();
    // frame5->showPage();
    //
    // buffer_manager.requestPage(6);
    // buffer_manager.requestPage(2);
    // buffer_manager.requestPage(5);
    // buffer_manager.requestPage(9);
    Frame* mainFrame;
    // menú para solicitar páginas (lectura y escritura (dirty true y modificar data)), liberar páginas, mostrar páginas y salir
    int option;
    do {
        std::cout << "1. Solicitar página\n";
        std::cout << "2. Liberar página\n";
        std::cout << "3. Mostrar páginas\n";
        std::cout << "4. Salir\n";
        std::cin >> option;
        switch (option) {
            case 1:
                int block_id;
                std::cout << "Ingrese el block_id: ";
                std::cin >> block_id;
                mainFrame = buffer_manager.requestPage(block_id);
                if(mainFrame == nullptr) {
                    std::cout << "No se pudo cargar la página\n";
                    break;
                }
                mainFrame->showPage();
                std::cout << "desea modificar la data? (1: si, 0: no): ";
                int mod;
                std::cin >> mod;
                if (mod == 1) {
                    std::string data;
                    std::cout << "Ingrese la nueva data: ";
                    mainFrame->page->data = data;
                    //cin de linea entera
                    std::cin.ignore();
                    std::getline(std::cin, data);
                    mainFrame->page->dirty = true;
                }
                break;
            case 2:
                int page_id;
                std::cout << "Ingrese el page_id: ";
                std::cin >> page_id;
                buffer_manager.releasePage(page_id,buffer_manager.getPage(page_id)->dirty);
                break;
            case 3:
                buffer_manager.showFrames();
                break;
            case 4:
                break;
            default:
                std::cout << "Opción inválida\n";
                break;
        }
    } while (option != 4);
    return 0;
}
