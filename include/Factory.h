#pragma once
#include <memory>
#include <string>

class Dragon;
class Frog;
class Knight;

class NPC;

// Factory для создания NPC
class NPCFactory {
public:
    static std::shared_ptr<NPC> createNPC(const std::string& type, const std::string& name, double x, double y);
};
