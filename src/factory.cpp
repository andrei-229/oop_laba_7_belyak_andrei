#include "factory.h"
#include "dragon.h"
#include "frog.h"
#include "knight.h"
#include <stdexcept>

std::shared_ptr<NPC> NPCFactory::createNPC(const std::string& type, const std::string& name, double x, double y) {
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        throw std::invalid_argument("Coordinates must be in range [0, 500]");
    }
    
    if (type == "Dragon" || type == "dragon" || type == "1") {
        return std::make_shared<Dragon>(name, x, y);
    } else if (type == "Frog" || type == "frog" || type == "2") {
        return std::make_shared<Frog>(name, x, y);
    } else if (type == "Knight" || type == "knight" || type == "3") {
        return std::make_shared<Knight>(name, x, y);
    }
    
    throw std::invalid_argument("Unknown NPC type: " + type);
}
