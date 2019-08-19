cmake_minimum_required(VERSION 3.14)
project(min_perdas)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS  "-O3")

include_directories(.)

add_executable(min_perdas
        Edge.cpp
        Edge.h
        Graph_network.cpp
        Graph_network.h
        main.cpp
        Evolutionary_path_relinking.cpp
        Evolutionary_path_relinking.h
        RK_Individual.cpp
        RK_Individual.h
        OS_Individual.h
        OS_Individual.cpp
        RKGA.cpp
        RKGA.h
        Vertex.cpp
        Vertex.h)
