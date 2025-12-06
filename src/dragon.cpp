#include "dragon.h"
#include "knight.h"
#include "visitor.h"

// Дракон: скорость 50 м/сек, дальность убийства 30 метров
Dragon::Dragon(const std::string& name, double x, double y) 
    : NPC(name, x, y, 50.0, 30.0) {}

std::string Dragon::getType() const {
    return "Dragon";
}

void Dragon::accept(Visitor& visitor) {
    visitor.visit(*this);
}

// Дракон убивает рыцарей
bool Dragon::canKill(const NPC& other) const {
    return other.getType() == "Knight";
}
