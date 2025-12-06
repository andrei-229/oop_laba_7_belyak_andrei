#pragma once
#include <memory>
#include <vector>

class Dragon;
class Frog;
class Knight;
class NPC;
class Observer;

// Visitor для боевого режима
class Visitor {
protected:
    std::vector<Observer*> observers;
    double range;
    
    void notify(const std::string& message);
    
public:
    Visitor(double range);
    virtual ~Visitor() = default;
    
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    
    virtual void visit(Dragon& dragon) = 0;
    virtual void visit(Frog& frog) = 0;
    virtual void visit(Knight& knight) = 0;
};
