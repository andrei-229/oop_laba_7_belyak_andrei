#pragma once
#include "observer.h"
#include <fstream>
#include <string>

// Наблюдатель для записи в файл
class FileObserver : public Observer {
private:
    std::string filename;
    
public:
    FileObserver(const std::string& filename);
    void onEvent(const std::string& message) override;
};
