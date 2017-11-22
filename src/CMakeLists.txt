cmake_minimum_required(VERSION 3.8)

project (Magma-Engine)

set(CMAKE_CXX_STANDARD 14)

set(LINK_EXTERNAL_LIBS
 optimized "glfw3" debug "glfw3d"
 optimized "glew32" debug "glew32d")

 
file(GLOB_RECURSE MAGMA_SRC
    "Magma/*.hpp"
    "Magma/*.cpp"
)

file(GLOB_RECURSE EXAMPLE_SRC
    "Example/*.hpp"
    "Example/*.cpp"
)

add_library(Engine ${MAGMA_SRC})
include_directories("../extern/src")

# Add Example executable
link_directories(Example "../extern/lib")
add_executable(Example ${EXAMPLE_SRC}) 
include_directories(".")
target_link_libraries(Example Engine)
target_link_libraries(Example ${LINK_EXTERNAL_LIBS})
