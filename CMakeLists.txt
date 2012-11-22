cmake_minimum_required(VERSION 2.8)
project(quads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable(quads main.cpp Object.cpp Quadtree.cpp)

target_link_libraries(quads -ltbb -lpthread -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio GL GLU GLEW )
