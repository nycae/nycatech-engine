cmake_minimum_required(VERSION 3.30)
project(nycatech_engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(protos)
add_subdirectory(engine)
add_subdirectory(entrypoint)