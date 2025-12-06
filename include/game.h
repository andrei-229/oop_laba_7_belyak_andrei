#pragma once
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <chrono>
#include <atomic>
#include "npc.h"

struct Battle {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
    
    Battle(std::shared_ptr<NPC> a, std::shared_ptr<NPC> d)
        : attacker(a), defender(d) {}
};

class Game {
private:
    int mapSize;                           // размер карты, задаётся пользователем
    static constexpr int INITIAL_NPCS = 50;
    static constexpr int GAME_DURATION_SEC = 30;
    
    std::vector<std::shared_ptr<NPC>> npcs;
    std::queue<Battle> battleQueue;
    
    mutable std::shared_mutex npcsMutex;           // shared_lock для чтения позиций
    mutable std::mutex battleQueueMutex;
    mutable std::mutex consoleMutex;              // lock_guard для cout
    
    std::atomic<bool> running;
    std::chrono::system_clock::time_point startTime;
    
public:
    Game();
    ~Game();
    
    void start();
    void stop();
    void setMapSize(int size);
    
private:
    // Создать начальное количество NPC
    void initializeNPCs();
    
    // Поток движения NPC
    void movementThread();
    
    // Поток обработки боев
    void battleThread();
    
    // Основной поток - вывод карты
    void renderThread();
    
    // Вспомогательные методы
    void printMap() const;
    bool isTimeToStop() const;
    void printSurvivors() const;
};
