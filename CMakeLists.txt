cmake_minimum_required(VERSION 3.21)
project(ATM-cpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(ATM-cpp
    src/main.cpp
    src/atmwindow.h
    src/atmwindow.cpp
    src/account.h
    src/account.cpp
)
target_link_libraries(ATM-cpp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(MSVC)
    target_compile_options(ATM-cpp PRIVATE /W4)
else()
    target_compile_options(ATM-cpp PRIVATE -Wall -Wextra -Wpedantic)
endif()
