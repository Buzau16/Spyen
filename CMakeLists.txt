cmake_minimum_required(VERSION 3.16)
project(Spyen)

set(CMAKE_CXX_STANDARD 20)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Include paths
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/src
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/vendor/glad/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/vendor/glfw/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/vendor/spdlog/include
)

# Source files

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Spyen/src/**/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Spyen/include/**/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Spyen/include/**/*.hpp")

# Add glad source manually
set(GLAD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/vendor/glad/src/glad.c)

# Build shared library
add_library(Spyen SHARED ${SOURCES} ${GLAD_SOURCE} ${HEADERS})

# Preprocessor defines
target_compile_definitions(Spyen PRIVATE
        SPYEN_BUILD_DLL
        $<$<CONFIG:Debug>:SP_DEBUG>
)

# Link OpenGL, GLFW, dl, pthread
target_link_libraries(Spyen PRIVATE
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARIES}
        dl
        pthread
)

target_include_directories(Spyen PRIVATE ${GLFW_INCLUDE_DIRS})

# ======================
# Sandbox
# ======================
project(Sandbox)

add_executable(Sandbox "SandBox/src/main.cpp"
        Spyen/include/Core/LogMacros.h
)

target_include_directories(Sandbox PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Spyen/include
)

target_link_libraries(Sandbox PRIVATE Spyen)
