cmake_minimum_required(VERSION 3.23)
project(GroebnerBasisLibrary)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(GroebnerBasisLibrary main.cpp)
add_executable(TestMonomialOrder MonomialOrdersTest.cpp)
