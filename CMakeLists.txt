cmake_minimum_required(VERSION 3.28.3)
project( tetris )

#include(/opt/vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( tetris 
                tetris.cpp 
                game.h
                game.cpp
                renderer.h
                renderer.cpp
                )
target_link_libraries( tetris ${OpenCV_LIBS} )
