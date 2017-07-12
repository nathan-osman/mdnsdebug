cmake_minimum_required(VERSION 3.2.0)
project(qmdnsdebug)

find_package(Qt5Network 5.4 REQUIRED)
find_package(qmdnsengine REQUIRED)

set(CMAKE_AUTOMOC ON)

add_subdirectory(src)
