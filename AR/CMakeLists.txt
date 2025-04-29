cmake_minimum_required(VERSION 4.0.1)

set(CMAKE_CXX_STANDARD 23)

project(ets2la_AR)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

pybind11_add_module(ets2la_AR main.cpp)