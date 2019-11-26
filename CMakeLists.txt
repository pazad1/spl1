cmake_minimum_required(VERSION 3.15)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)

add_executable(spl1 src/Main.cpp include/Action.h include/json.hpp include/Session.h include/User.h include/Watchable.h src/Action.cpp)