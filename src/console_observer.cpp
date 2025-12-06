#include "console_observer.h"

void ConsoleObserver::onEvent(const std::string& message) {
    std::cout << "[Console] " << message << std::endl;
}
