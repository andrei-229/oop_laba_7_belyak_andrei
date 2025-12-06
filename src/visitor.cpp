#include "visitor.h"
#include "observer.h"
#include <algorithm>

Visitor::Visitor(double range) : range(range) {}

void Visitor::addObserver(Observer* observer) {
    observers.push_back(observer);
}

void Visitor::removeObserver(Observer* observer) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
}

void Visitor::notify(const std::string& message) {
    for (auto observer : observers) {
        observer->onEvent(message);
    }
}
