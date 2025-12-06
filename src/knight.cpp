#include "knight.h"
#include "dragon.h"
#include "visitor.h"

// Рыцарь: скорость 30 м/сек, дальность убийства 10 метров
Knight::Knight(const std::string& name, double x, double y) 
    : NPC(name, x, y, 30.0, 10.0) {}

std::string Knight::getType() const {
    return "Knight";
}

void Knight::accept(Visitor& visitor) {
    visitor.visit(*this);
}

// Рыцарь убивает драконов
bool Knight::canKill(const NPC& other) const {
    return other.getType() == "Dragon";
}
