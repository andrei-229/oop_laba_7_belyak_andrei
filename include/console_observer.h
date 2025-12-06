#pragma once
#include "observer.h"
#include <iostream>

// Наблюдатель для вывода на консоль
class ConsoleObserver : public Observer {
public:
    void onEvent(const std::string& message) override;
};
