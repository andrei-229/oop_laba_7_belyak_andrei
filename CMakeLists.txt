cmake_minimum_required(VERSION 3.10)
project(DungeonEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем директорию с заголовочными файлами
include_directories(include)

# Исходные файлы
set(SOURCES
    src/main.cpp
    src/npc.cpp
    src/dragon.cpp
    src/frog.cpp
    src/knight.cpp
    src/game.cpp
    src/factory.cpp
    src/visitor.cpp
    src/battle_visitor.cpp
    src/console_observer.cpp
    src/file_observer.cpp
    src/editor.cpp
)

# Заголовочные файлы
set(HEADERS
    include/npc.h
    include/dragon.h
    include/frog.h
    include/knight.h
    include/game.h
    include/factory.h
    include/visitor.h
    include/battle_visitor.h
    include/observer.h
    include/console_observer.h
    include/file_observer.h
    include/editor.h
)

# Создание исполняемого файла
add_executable(dungeon_editor ${SOURCES} ${HEADERS})

# Поддержка многопоточности
find_package(Threads REQUIRED)
target_link_libraries(dungeon_editor PRIVATE Threads::Threads)

# Установка предупреждений компилятора
if(MSVC)
    target_compile_options(dungeon_editor PRIVATE /W4)
else()
    target_compile_options(dungeon_editor PRIVATE -Wall -Wextra -pedantic)
endif()

