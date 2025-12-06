#pragma once
#include "npc.h"

class Knight : public NPC {
public:
    Knight(const std::string& name, double x, double y);
    
    std::string getType() const override;
    void accept(Visitor& visitor) override;
    bool canKill(const NPC& other) const override;
};
