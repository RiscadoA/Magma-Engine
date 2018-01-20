cmake_minimum_required(VERSION 3.8)

# Add external dependencies
add_subdirectory(extern/)

project (Magma-Engine)
set(CMAKE_CXX_STANDARD 14)

# Add engine and example source
add_subdirectory(src/)