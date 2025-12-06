#pragma once
#include "npc.h"

class Frog : public NPC {
public:
    Frog(const std::string& name, double x, double y);
    
    std::string getType() const override;
    void accept(Visitor& visitor) override;
    bool canKill(const NPC& other) const override;
};
