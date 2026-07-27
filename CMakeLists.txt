cmake_minimum_required(VERSION 3.16)

project(ChronoCLI
    VERSION 1.0.0
    DESCRIPTION "A C++ terminal calendar and event manager"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(chronocli_core
    src/Calendar.cpp
    src/InputUtils.cpp
    src/DateUtils.cpp
    src/EventManager.cpp
    src/TerminalUI.cpp
)

target_include_directories(chronocli_core PUBLIC include)

if(MSVC)
    target_compile_options(chronocli_core PRIVATE /W4)
else()
    target_compile_options(
        chronocli_core
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
    )
endif()

add_executable(chronocli
    src/main.cpp
)

target_link_libraries(chronocli PRIVATE chronocli_core)

add_executable(chronocli_tests
    tests/test_runner.cpp
)

target_link_libraries(chronocli_tests PRIVATE chronocli_core)

add_test(NAME ChronoCLITests COMMAND chronocli_tests)
