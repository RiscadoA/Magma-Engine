cmake_minimum_required(VERSION 3.8)

project (Magma-Engine)

set(CMAKE_CXX_STANDARD 14)

#set(LINK_EXTERNAL_LIBS
# optimized "glfw3" debug "glfw3d"
# optimized "glew32" debug "glew32d")

 
file(GLOB_RECURSE MAGMA_SRC
    "Magma/*.hpp"
    "Magma/*.cpp"
)

file(GLOB_RECURSE EXAMPLE_SRC
    "Example/*.hpp"
    "Example/*.cpp"
)

add_library(Engine ${MAGMA_SRC})
include_directories("extern/src")

# Include GLFW
add_subdirectory("extern/glfw-3.2.1")
include_directories("extern/glfw-3.2.1/include")
target_link_libraries(Engine glfw ${GLFW_LIBRARIES})

# Include GLEW
add_subdirectory("extern/glew-2.0.0")
include_directories("extern/glew-2.0.0/include")
target_link_libraries(Engine glew_s ${GLEW_LIBRARIES})

# Add Example executable
link_directories(Example "extern/lib")
add_executable(Example ${EXAMPLE_SRC}) 
include_directories(".")
target_link_libraries(Example Engine)
target_link_libraries(Example ${LINK_EXTERNAL_LIBS})
