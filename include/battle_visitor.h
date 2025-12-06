#pragma once
#include "visitor.h"
#include <vector>
#include <memory>

class NPC;

// Конкретный Visitor для проведения боя
class BattleVisitor : public Visitor {
private:
    std::vector<std::shared_ptr<NPC>>* npcs;
    NPC* currentAttacker;
    
public:
    BattleVisitor(double range, std::vector<std::shared_ptr<NPC>>* npcs);
    
    void setCurrentAttacker(NPC* attacker);
    
    void visit(Dragon& dragon) override;
    void visit(Frog& frog) override;
    void visit(Knight& knight) override;
};
