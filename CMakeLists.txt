cmake_minimum_required(VERSION 3.16)
project(SmartTimerCpp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

add_executable(smart_timer
        src/main.cpp
        src/MainWindow.cpp
        src/Timer.cpp
        src/TimerManager.cpp
        src/AddEditDialog.cpp
)

target_link_libraries(smart_timer PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
