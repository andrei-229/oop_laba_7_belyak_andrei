#include "battle_visitor.h"
#include "dragon.h"
#include "frog.h"
#include "knight.h"
#include "npc.h"
#include <random>

BattleVisitor::BattleVisitor(double range, std::vector<std::shared_ptr<NPC>>* npcs) 
    : Visitor(range), npcs(npcs), currentAttacker(nullptr) {}

void BattleVisitor::setCurrentAttacker(NPC* attacker) {
    currentAttacker = attacker;
}

void BattleVisitor::visit(Dragon& dragon) {
    if (!currentAttacker || !currentAttacker->isAlive() || !dragon.isAlive()) {
        return;
    }
    
    double distance = currentAttacker->distanceTo(dragon);
    if (distance > range) {
        return;
    }
    
    std::string attackerType = currentAttacker->getType();
    
    // Правила боя:
    // Frog ест всех (включая жаб)
    // Knight убивает Dragon
    // Dragon убивает Knight
    
    if (attackerType == "Frog") {
        // Жаба ест всех
        dragon.kill();
        notify(currentAttacker->getName() + " (Frog) съел " + dragon.getName() + " (Dragon)");
    } else if (attackerType == "Knight") {
        // Рыцарь убивает дракона
        dragon.kill();
        notify(currentAttacker->getName() + " (Knight) убил " + dragon.getName() + " (Dragon)");
    } else if (attackerType == "Dragon") {
        // Дракон не может убить другого дракона
        // Ничья - оба погибают
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        
        if (dis(gen) == 0) {
            dragon.kill();
            currentAttacker->kill();
            notify(dragon.getName() + " (Dragon) и " + currentAttacker->getName() + " (Dragon) убили друг друга");
        }
    }
}

void BattleVisitor::visit(Frog& frog) {
    if (!currentAttacker || !currentAttacker->isAlive() || !frog.isAlive()) {
        return;
    }
    
    double distance = currentAttacker->distanceTo(frog);
    if (distance > range) {
        return;
    }
    
    std::string attackerType = currentAttacker->getType();
    
    if (attackerType == "Frog") {
        // Жаба ест жабу
        frog.kill();
        notify(currentAttacker->getName() + " (Frog) съел " + frog.getName() + " (Frog)");
    } else if (attackerType == "Knight") {
        // Рыцарь не может убить жабу, но жаба не ест рыцаря сразу
        // Ничья - оба погибают
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        
        if (dis(gen) == 0) {
            frog.kill();
            currentAttacker->kill();
            notify(frog.getName() + " (Frog) и " + currentAttacker->getName() + " (Knight) убили друг друга");
        }
    } else if (attackerType == "Dragon") {
        // Дракон не может убить жабу
        // Ничья - оба погибают
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        
        if (dis(gen) == 0) {
            frog.kill();
            currentAttacker->kill();
            notify(frog.getName() + " (Frog) и " + currentAttacker->getName() + " (Dragon) убили друг друга");
        }
    }
}

void BattleVisitor::visit(Knight& knight) {
    if (!currentAttacker || !currentAttacker->isAlive() || !knight.isAlive()) {
        return;
    }
    
    double distance = currentAttacker->distanceTo(knight);
    if (distance > range) {
        return;
    }
    
    std::string attackerType = currentAttacker->getType();
    
    if (attackerType == "Frog") {
        // Жаба ест рыцаря
        knight.kill();
        notify(currentAttacker->getName() + " (Frog) съел " + knight.getName() + " (Knight)");
    } else if (attackerType == "Knight") {
        // Рыцарь не убивает рыцаря
        // Ничья - оба погибают
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        
        if (dis(gen) == 0) {
            knight.kill();
            currentAttacker->kill();
            notify(knight.getName() + " (Knight) и " + currentAttacker->getName() + " (Knight) убили друг друга");
        }
    } else if (attackerType == "Dragon") {
        // Дракон убивает рыцаря
        knight.kill();
        notify(currentAttacker->getName() + " (Dragon) убил " + knight.getName() + " (Knight)");
    }
}
