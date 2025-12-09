cmake_minimum_required(VERSION 3.20)
project(meshpack_lib CXX)

add_library(hb_meshpack STATIC
  meshpack_lib.cpp
)

target_include_directories(hb_meshpack PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}                         # meshpack_lib.hpp
  ${CMAKE_SOURCE_DIR}/engine/extern/cgltf       # cgltf.h
)

#inherit engine include dirs like "hobgoblin/..."
target_link_libraries(hb_meshpack PUBLIC hobgoblin)
