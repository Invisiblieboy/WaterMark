cmake_minimum_required(VERSION 3.24)
SET(CMAKE_CXX_STANDARD 23)

project(WaterMark LANGUAGES CXX)

enable_testing()

add_executable(${PROJECT_NAME}
  main.cpp
  ArgumentParser.cpp
)

add_subdirectory(tests)
