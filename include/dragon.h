#pragma once
#include "npc.h"

class Dragon : public NPC {
public:
    Dragon(const std::string& name, double x, double y);
    
    std::string getType() const override;
    void accept(Visitor& visitor) override;
    bool canKill(const NPC& other) const override;
};
