cmake_minimum_required(VERSION 3.21)
project(dircleaner)

set(CMAKE_CXX_STANDARD 17)

add_executable(dircleaner main.cpp config.cpp cleaner.cpp message.cpp wildcard_matcher.cpp thread_canceller.cpp)
