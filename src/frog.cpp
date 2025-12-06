#include "frog.h"
#include "visitor.h"

// Жаба: скорость 1 м/сек, дальность убийства 10 метров
Frog::Frog(const std::string& name, double x, double y) 
    : NPC(name, x, y, 1.0, 10.0) {}

std::string Frog::getType() const {
    return "Frog";
}

void Frog::accept(Visitor& visitor) {
    visitor.visit(*this);
}

// Жаба ест всех (включая других жаб)
bool Frog::canKill(const NPC& /* other */) const {
    return true;
}
