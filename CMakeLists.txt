cmake_minimum_required(VERSION 3.30)
project(kodowanie_huffmana)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
  Core
  Gui
  Widgets
  REQUIRED)

add_executable(kodowanie_huffmana main.cpp
        FileManager.cpp
        FileManager.h
        HuffmanCodec.cpp
        HuffmanCodec.h)
target_link_libraries(kodowanie_huffmana
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
)

