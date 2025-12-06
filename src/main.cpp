#include <iostream>
#include "game.h"

int main() {
    std::cout << "╔═══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║        Лабораторная работа №6 - ООП          ║" << std::endl;
    std::cout << "║      Многопоточная боевая симуляция NPC      ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════╝" << std::endl;
    
    int mapSize;
    std::cout << "\nВведите размер карты (площадь, рекомендуется 50-200): ";
    std::cin >> mapSize;
    
    if (mapSize <= 0) {
        std::cout << "Некорректный размер карты. Используется размер по умолчанию: 100" << std::endl;
        mapSize = 100;
    }
    
    std::cout << "\nПравила боя:" << std::endl;
    std::cout << "  • Дракон (D): скорость 50 м/с, убивает Рыцарей, дистанция 30м" << std::endl;
    std::cout << "  • Жаба (F): скорость 1 м/с, ест всех, дистанция 10м" << std::endl;
    std::cout << "  • Рыцарь (K): скорость 30 м/с, убивает Драконов, дистанция 10м" << std::endl;
    std::cout << "\nСимуляция запускается сейчас...\n" << std::endl;
    
    Game game;
    game.setMapSize(mapSize);
    game.start();
    
    return 0;
}

