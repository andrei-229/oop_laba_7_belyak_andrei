#pragma once
#include <vector>
#include <memory>
#include <string>
#include "npc.h"
#include "observer.h"

class Editor {
private:
    std::vector<std::shared_ptr<NPC>> npcs;
    std::vector<std::unique_ptr<Observer>> observers;
    
public:
    Editor();
    ~Editor();
    
    // Добавить нового NPC
    void addNPC(const std::string& type, const std::string& name, double x, double y);
    
    // Сохранить в файл
    void saveToFile(const std::string& filename);
    
    // Загрузить из файла
    void loadFromFile(const std::string& filename);
    
    // Печать всех NPC
    void printAll() const;
    
    // Запуск боевого режима
    void startBattle(double range);
    
    // Проверка уникальности имени
    bool isNameUnique(const std::string& name) const;
};
