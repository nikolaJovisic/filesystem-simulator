cmake_minimum_required(VERSION 3.21)
project(filesystem_simulator)

set(CMAKE_CXX_STANDARD 23)

add_executable(filesystem_simulator main.cpp PersistentStorage.cpp PersistentStorage.h)
