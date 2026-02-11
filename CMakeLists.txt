cmake_minimum_required(VERSION 3.15)

# add c++ runtime lib statically to .exe file
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# ///////// GLF Project
project(GLF)

set(CMAKE_CXX_STANDARD 20) #c++ version
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WIN32_EXECUTABLE ON)

file(GLOB_RECURSE GLF_SOURCE_FILES "GLF/*.cpp" "GLF/*.h")

add_executable(GLF ${GLF_SOURCE_FILES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/GLF" 
             PREFIX "" 
             FILES ${GLF_SOURCE_FILES})

target_include_directories(GLF PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/GLF)

# Macros
target_compile_definitions(GLF PUBLIC ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
# target_compile_definitions(GLF PUBLIC ASSETS_DIR = ./assets/)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) #static linked

add_subdirectory(dep/GLFW)

# GLAD
add_subdirectory(dep/GLAD)

#GLM
add_subdirectory(dep/GLM)

#IMGUI
add_subdirectory(dep/IMGUI)

#assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DOCS OFF CACHE BOOL "" FORCE)

set(ASSIMP_ASAN_RELEASE OFF CACHE BOOL "" FORCE)
set(ASSIMP_UBSAN_RELEASE OFF CACHE BOOL "" FORCE)
set(ASSIMP_ASAN_RELEASE OFF CACHE BOOL "" FORCE)
set(ASSIMP_ASAN_DEBUG OFF CACHE BOOL "" FORCE)

set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)

add_subdirectory(dep/ASSIMP)

# Link
target_link_libraries(GLF PRIVATE glfw glad glm::glm assimp imgui imgui_impl_glfw imgui_impl_opengl3)

# Windows
if(WIN32)
    set_target_properties(GLF PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
endif()