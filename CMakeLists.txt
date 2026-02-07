cmake_minimum_required(VERSION 3.15)
project(GLF)

# C++ standard
set(CMAKE_CXX_STANDARD 20) #c++ version
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#GLFW
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

# GLFW - Static linking
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) #static linked

FetchContent_MakeAvailable(glfw)

# project
add_executable(GLF 
    GLF/main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC GLF)

# link GLFW
target_link_libraries(GLF PRIVATE glfw)

# Windows
if(WIN32)
    set_target_properties(GLF PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
endif()