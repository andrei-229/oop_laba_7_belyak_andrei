#include "file_observer.h"

FileObserver::FileObserver(const std::string& filename) : filename(filename) {}

void FileObserver::onEvent(const std::string& message) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << message << std::endl;
        file.close();
    }
}
