cmake_minimum_required(VERSION 3.16)

project(ChronoCLI
    VERSION 1.0.0
    DESCRIPTION "A C++ terminal calendar and event manager"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(chronocli
    src/main.cpp
    src/Calendar.cpp
    src/InputUtils.cpp
    src/DateUtils.cpp
    src/EventManager.cpp
)

target_include_directories(chronocli PRIVATE include)