#include "editor.h"
#include "factory.h"
#include "battle_visitor.h"
#include "console_observer.h"
#include "file_observer.h"
#include <fstream>
#include <iostream>
#include <algorithm>

Editor::Editor() {
    // Создаем наблюдателей
    observers.push_back(std::make_unique<ConsoleObserver>());
    observers.push_back(std::make_unique<FileObserver>("log.txt"));
}

Editor::~Editor() = default;

bool Editor::isNameUnique(const std::string& name) const {
    for (const auto& npc : npcs) {
        if (npc->getName() == name) {
            return false;
        }
    }
    return true;
}

void Editor::addNPC(const std::string& type, const std::string& name, double x, double y) {
    if (!isNameUnique(name)) {
        throw std::invalid_argument("NPC with name '" + name + "' already exists!");
    }
    
    auto npc = NPCFactory::createNPC(type, name, x, y);
    npcs.push_back(npc);
    std::cout << "NPC '" << name << "' (" << npc->getType() << ") добавлен на координаты (" 
              << x << ", " << y << ")" << std::endl;
}

void Editor::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            npc->save(file);
        }
    }
    
    file.close();
    std::cout << "NPC сохранены в файл: " << filename << std::endl;
}

void Editor::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }
    
    npcs.clear();
    
    while (file) {
        auto npc = NPC::load(file);
        if (npc) {
            npcs.push_back(npc);
        }
    }
    
    file.close();
    std::cout << "Загружено " << npcs.size() << " NPC из файла: " << filename << std::endl;
}

void Editor::printAll() const {
    if (npcs.empty()) {
        std::cout << "Нет NPC для отображения." << std::endl;
        return;
    }
    
    std::cout << "\n=== Список NPC ===" << std::endl;
    std::cout << "Всего NPC: " << npcs.size() << std::endl;
    
    int alive = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            alive++;
            std::cout << "  [" << npc->getType() << "] " 
                     << npc->getName() 
                     << " - координаты: (" << npc->getX() << ", " << npc->getY() << ")"
                     << " - статус: жив" << std::endl;
        } else {
            std::cout << "  [" << npc->getType() << "] " 
                     << npc->getName() 
                     << " - координаты: (" << npc->getX() << ", " << npc->getY() << ")"
                     << " - статус: мертв" << std::endl;
        }
    }
    std::cout << "Живых NPC: " << alive << std::endl;
    std::cout << "==================\n" << std::endl;
}

void Editor::startBattle(double range) {
    std::cout << "\n=== НАЧАЛО БОЕВОГО РЕЖИМА ===" << std::endl;
    std::cout << "Дальность боя: " << range << " метров" << std::endl;
    
    // Создаем visitor для боя
    BattleVisitor visitor(range, &npcs);
    
    // Подключаем всех наблюдателей
    for (auto& observer : observers) {
        visitor.addObserver(observer.get());
    }
    
    // Каждый живой NPC атакует всех остальных живых NPC в радиусе действия
    bool anyKilled = true;
    int round = 1;
    
    while (anyKilled) {
        anyKilled = false;
        std::cout << "\n--- Раунд " << round << " ---" << std::endl;
        
        for (auto& attacker : npcs) {
            if (!attacker->isAlive()) {
                continue;
            }
            
            visitor.setCurrentAttacker(attacker.get());
            
            for (auto& defender : npcs) {
                if (attacker == defender || !defender->isAlive()) {
                    continue;
                }
                
                bool wasAlive = defender->isAlive();
                defender->accept(visitor);
                
                if (wasAlive && !defender->isAlive()) {
                    anyKilled = true;
                }
            }
        }
        
        // Удаляем мертвых NPC из списка
        auto initialSize = npcs.size();
        npcs.erase(
            std::remove_if(npcs.begin(), npcs.end(), 
                [](const std::shared_ptr<NPC>& npc) { return !npc->isAlive(); }),
            npcs.end()
        );
        
        if (npcs.size() < initialSize) {
            anyKilled = true;
        }
        
        if (!anyKilled) {
            std::cout << "Больше нет сражений" << std::endl;
            break;
        }
        
        round++;
    }
    
    std::cout << "\n=== КОНЕЦ БОЕВОГО РЕЖИМА ===" << std::endl;
    std::cout << "Выживших NPC: " << npcs.size() << std::endl;
}
