cmake_minimum_required(VERSION 3.31)
project(Spyen)

find_package(OpenGL REQUIRED)


set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Spyen/src/*.cpp")

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Spyen/include/*.h")

# Add the Spyen library
add_library(Spyen SHARED ${SOURCES} ${HEADERS})

# Define SPYEN_BUILD when building the DLL itself
target_compile_definitions(Spyen PRIVATE SPYEN_BUILD_DLL)

# Make Spyen's headers available
target_include_directories(Spyen
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/src
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/vendor/glad/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/vendor/glfw/include
)


target_link_directories(Spyen
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/vendor/glfw/lib
)

# Option 1: Add glad.c directly to your Spyen target
target_sources(Spyen PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/vendor/glad/src/glad.c  # Adjust path to where your glad.c is
)

# Add include directory for glad headers
target_include_directories(Spyen PRIVATE
    ${YOUR_GLAD_INCLUDE_DIR}  # Adjust path to where glad.h is located
)

# And link with OpenGL and GLFW
target_link_libraries(Spyen PRIVATE
    glfw3dll
    opengl32
)

project(SandBox)
# Add the Sandbox executable (note: case sensitivity matters)
add_executable(Sandbox "SandBox/src/main.cpp")

target_include_directories(
        Sandbox PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/include
)

# Link Sandbox with Spyen
target_link_libraries(Sandbox PRIVATE Spyen)