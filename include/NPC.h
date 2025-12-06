#pragma once
#include <string>
#include <memory>
#include <cmath>
#include <random>

class Visitor;

class NPC {
protected:
    std::string name;
    double x, y;
    bool alive;
    double speed;              // скорость передвижения (м/сек)
    double killDistance;       // дистанция убийства (метры)

public:
    NPC(const std::string& name, double x, double y, double speed, double killDistance);
    virtual ~NPC() = default;

    virtual std::string getType() const = 0;
    virtual void accept(Visitor& visitor) = 0;
    
    std::string getName() const;
    double getX() const;
    double getY() const;
    bool isAlive() const;
    void kill();
    double getSpeed() const;
    double getKillDistance() const;
    
    // Передвижение NPC
    void moveTo(double newX, double newY);
    
    // Проверка, может ли этот NPC убить другого
    virtual bool canKill(const NPC& other) const = 0;
    
    // Получить урон (кубик 1-6)
    int rollDice() const;
    
    double distanceTo(const NPC& other) const;
    
    virtual void save(std::ostream& os) const;
    static std::shared_ptr<NPC> load(std::istream& is);
};
