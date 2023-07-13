cmake_minimum_required(VERSION 3.9)
project(meows
    VERSION 0.0.0 
    LANGUAGES CXX
    DESCRIPTION "The labricecat language collection"
)
set(CMAKE_CXX_STANDARD 20)
set(BINARY meows)
set(SOURCE 
    meows.cpp
    )

add_executable(${BINARY} ${SOURCE})
install(TARGETS ${BINARY})
