cmake_minimum_required(VERSION 3.12)

project(
        labwork3
        VERSION 1.0
        DESCRIPTION "endless sandpile model"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
add_executable("Sandpile" "main.cpp")
