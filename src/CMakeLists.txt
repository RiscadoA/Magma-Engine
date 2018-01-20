file(GLOB_RECURSE MAGMA_SRC
    "Magma/*.hpp"
    "Magma/*.cpp"
)

file(GLOB_RECURSE EXAMPLE_SRC
    "Example/*.hpp"
    "Example/*.cpp"
)

add_library(Engine ${MAGMA_SRC})

# Include GLM
include_directories(../extern/glm/)

# Include GLFW
include_directories(../extern/glfw/include)
target_link_libraries(Engine glfw ${GLFW_LIBRARIES})

# Include GLEW
include_directories(../extern/glew/include)
target_link_libraries(Engine glew_s ${GLEW_LIBRARIES})

# Add Example executable
add_executable(Example ${EXAMPLE_SRC}) 
include_directories(".")
target_link_libraries(Example Engine)
target_link_libraries(Example ${LINK_EXTERNAL_LIBS})

option(MAGMA_USE_DIRECTX "If off, engine uses OpenGL, with GLEW and GLFW. Otherwise, engine uses DirectX and Windows")
if(MAGMA_USE_DIRECTX)
add_definitions(-DMAGMA_USING_DIRECTX)
SET_TARGET_PROPERTIES (Example PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
SET_TARGET_PROPERTIES (Example PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
SET_TARGET_PROPERTIES (Example PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
SET_TARGET_PROPERTIES (Example PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows")
endif(MAGMA_USE_DIRECTX)

set(MAGMA_RESOURCES_PATH "../resources" CACHE PATH "Sets the Resources folder path")
add_definitions(-DMAGMA_RESOURCES_PATH="${MAGMA_RESOURCES_PATH}")
