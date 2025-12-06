#include "game.h"
#include "dragon.h"
#include "frog.h"
#include "knight.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <cstring>
#include <thread>
#include <chrono>

Game::Game() : mapSize(100), running(false) {}

Game::~Game() {
    stop();
}

void Game::initializeNPCs() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> coordDis(0.0, static_cast<double>(mapSize - 1));
    std::uniform_int_distribution<> typeDis(0, 2);
    
    for (int i = 0; i < INITIAL_NPCS; ++i) {
        double x = coordDis(gen);
        double y = coordDis(gen);
        std::string name = "NPC_" + std::to_string(i);
        
        std::shared_ptr<NPC> npc;
        int type = typeDis(gen);
        switch (type) {
            case 0:
                npc = std::make_shared<Dragon>(name, x, y);
                break;
            case 1:
                npc = std::make_shared<Frog>(name, x, y);
                break;
            case 2:
                npc = std::make_shared<Knight>(name, x, y);
                break;
        }
        
        npcs.push_back(npc);
    }
    
    {
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cout << "Инициализировано " << INITIAL_NPCS << " NPC на карте " 
                  << mapSize << "x" << mapSize << std::endl;
    }
}

void Game::movementThread() {
    std::mt19937 gen(std::random_device{}());
    
    while (running) {
        {
            std::lock_guard<std::shared_mutex> lock(npcsMutex);
            
            for (auto& npc : npcs) {
                if (!npc->isAlive()) continue;
                
                // Случайное направление и движение
                std::uniform_real_distribution<> angleDis(0, 2 * M_PI);
                std::uniform_real_distribution<> distDis(0, npc->getSpeed());
                
                double angle = angleDis(gen);
                double distance = distDis(gen);
                
                double newX = npc->getX() + distance * std::cos(angle);
                double newY = npc->getY() + distance * std::sin(angle);
                
                // Ограничить карту
                newX = std::max(0.0, std::min((double)mapSize - 1, newX));
                newY = std::max(0.0, std::min((double)mapSize - 1, newY));
                
                npc->moveTo(newX, newY);
                
                // Проверка на врагов в пределах дистанции убийства
                for (auto& other : npcs) {
                    if (other == npc || !other->isAlive() || !npc->isAlive()) continue;
                    
                    if (npc->canKill(*other) && npc->distanceTo(*other) <= npc->getKillDistance()) {
                        // Добавить в очередь боев
                        {
                            std::lock_guard<std::mutex> battleLock(battleQueueMutex);
                            battleQueue.emplace(npc, other);
                        }
                    }
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Game::battleThread() {
    while (running) {
        Battle battle(nullptr, nullptr);
        
        {
            std::lock_guard<std::mutex> lock(battleQueueMutex);
            if (!battleQueue.empty()) {
                battle = battleQueue.front();
                battleQueue.pop();
            }
        }
        
        if (battle.attacker && battle.defender) {
            if (battle.attacker->isAlive() && battle.defender->isAlive()) {
                int attackPower = battle.attacker->rollDice();
                int defensePower = battle.defender->rollDice();
                
                {
                    std::lock_guard<std::mutex> lock(consoleMutex);
                    std::cout << "[БОЙ] " << battle.attacker->getName() 
                              << "(" << battle.attacker->getType() << ") [" << attackPower << "] vs "
                              << battle.defender->getName() 
                              << "(" << battle.defender->getType() << ") [" << defensePower << "]";
                    
                    if (attackPower > defensePower) {
                        std::cout << " -> Погиб " << battle.defender->getName() << std::endl;
                        battle.defender->kill();
                    } else if (defensePower > attackPower) {
                        std::cout << " -> Погиб " << battle.attacker->getName() << std::endl;
                        battle.attacker->kill();
                    } else {
                        std::cout << " -> Ничья, оба погибают!" << std::endl;
                        battle.attacker->kill();
                        battle.defender->kill();
                    }
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Game::renderThread() {
    auto lastRender = std::chrono::system_clock::now();
    
    while (running) {
        auto now = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastRender);
        
        if (elapsed.count() >= 1) {
            {
                std::lock_guard<std::shared_mutex> lock(npcsMutex);
                printMap();
            }
            lastRender = now;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Game::printMap() const {
    std::lock_guard<std::mutex> lock(consoleMutex);
    
    // Размер отображаемой карты
    int displaySize = std::min(20, std::max(5, mapSize / 5));
    int cellSize = std::max(1, mapSize / displaySize);
    
    std::cout << "\n╔";
    for (int i = 0; i < displaySize; ++i) std::cout << "═";
    std::cout << "╗" << std::endl;
    
    std::cout << "║ Карта (" << mapSize << "x" << mapSize << ") размер экрана: " 
              << displaySize << "x" << displaySize << " ║" << std::endl;
    
    std::cout << "╠";
    for (int i = 0; i < displaySize; ++i) std::cout << "═";
    std::cout << "╣" << std::endl;
    
    // Создаём динамическую карту
    std::vector<std::vector<char>> map(displaySize, std::vector<char>(displaySize, '.'));
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            int mapX = static_cast<int>(npc->getX()) / cellSize;
            int mapY = static_cast<int>(npc->getY()) / cellSize;
            
            if (mapX >= 0 && mapX < displaySize && mapY >= 0 && mapY < displaySize) {
                if (npc->getType() == "Dragon") {
                    map[mapY][mapX] = 'D';
                } else if (npc->getType() == "Frog") {
                    map[mapY][mapX] = 'F';
                } else if (npc->getType() == "Knight") {
                    map[mapY][mapX] = 'K';
                }
            }
        }
    }
    
    for (int y = 0; y < displaySize; ++y) {
        std::cout << "║";
        for (int x = 0; x < displaySize; ++x) {
            std::cout << map[y][x];
        }
        std::cout << "║" << std::endl;
    }
    
    std::cout << "╚";
    for (int i = 0; i < displaySize; ++i) std::cout << "═";
    std::cout << "╝" << std::endl;
    
    // Подсчет живых и мертвых
    int alive = 0, dead = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) alive++;
        else dead++;
    }
    
    std::cout << "Живых: " << alive << " | Мертвых: " << dead << std::endl;
}

bool Game::isTimeToStop() const {
    auto elapsed = std::chrono::system_clock::now() - startTime;
    return std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() >= GAME_DURATION_SEC;
}

void Game::printSurvivors() const {
    std::lock_guard<std::mutex> lock(consoleMutex);
    
    std::cout << "\n╔════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         КОНЕЦ СИМУЛЯЦИИ (30 секунд)       ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════╝" << std::endl;
    
    std::cout << "\nВыжившие NPC:\n";
    
    int dragonCount = 0, frogCount = 0, knightCount = 0;
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            std::cout << "  - " << npc->getName() << " (" << npc->getType() 
                      << ") на позиции (" << std::fixed << std::setprecision(1)
                      << npc->getX() << ", " << npc->getY() << ")" << std::endl;
            
            if (npc->getType() == "Dragon") dragonCount++;
            else if (npc->getType() == "Frog") frogCount++;
            else if (npc->getType() == "Knight") knightCount++;
        }
    }
    
    std::cout << "\nПодсчет выживших:\n";
    std::cout << "  Драконов: " << dragonCount << std::endl;
    std::cout << "  Жаб: " << frogCount << std::endl;
    std::cout << "  Рыцарей: " << knightCount << std::endl;
    std::cout << "  ВСЕГО: " << (dragonCount + frogCount + knightCount) 
              << " из " << INITIAL_NPCS << std::endl;
}

void Game::start() {
    if (running) return;
    
    running = true;
    startTime = std::chrono::system_clock::now();
    
    initializeNPCs();
    
    std::thread movement([this] { movementThread(); });
    std::thread battle([this] { battleThread(); });
    std::thread render([this] { renderThread(); });
    
    // Основной поток - проверка времени
    while (running && !isTimeToStop()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    running = false;
    
    // Ожидание завершения потоков
    movement.join();
    battle.join();
    render.join();
    
    printSurvivors();
}

void Game::stop() {
    running = false;
}

void Game::setMapSize(int size) {
    if (size > 0) {
        mapSize = size;
    }
}
