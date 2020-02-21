cmake_minimum_required(VERSION 3.14)
project(min_perdas)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS  "-O3")

include_directories(.)

add_executable(min_perdas
        source/Edge.cpp
        include/Edge.h
        source/Graph.cpp
        include/Graph.h
        main.cpp
        source/Evolutionary_path_relinking.cpp
        include/Evolutionary_path_relinking.h
        source/RK_Individual.cpp
        include/RK_Individual.h
        include/Individual.h
        source/Individual.cpp
        source/Random_keys.cpp
        include/Random_keys.h
        source/Vertex.cpp
        include/Vertex.h)
