cmake_minimum_required(VERSION 3.14)
project(minimizacao_perdas_sistema_distribuicao)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(minimizacao_perdas_sistema_distribuicao
        Edge.cpp
        Edge.h
        Graph.cpp
        Graph.h
        Individual.cpp
        Individual.h
        main.cpp
        Vertex.cpp
        Vertex.h
        Random_key.cpp
        Random_keys.h)
