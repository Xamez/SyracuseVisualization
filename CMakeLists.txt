cmake_minimum_required(VERSION 3.15)

project(SyracuseVisualization CXX)

add_executable(SyracuseVisualization main.cpp olcPixelGameEngine.h)

include(cmake/PGE.cmake)
pge_set_emscripten_options(main)