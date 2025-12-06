#pragma once
#include <string>

class Dragon;
class Frog;
class Knight;

// Базовый класс наблюдателя
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onEvent(const std::string& message) = 0;
};
