cmake_minimum_required(VERSION 3.31)
project(Vocksel LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.3.8
)


# Fetch and configure GLM (OpenGL Mathematics)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glfw glm)

add_library(glad STATIC third_party/glad/glad.c)
target_include_directories(glad PUBLIC include)

find_package(OpenGL REQUIRED)

add_executable(Vocksel
        src/main.cpp
)
target_link_libraries(Vocksel
        PRIVATE
        OpenGL::GL
        glad
        glfw
)
# Include directories
target_include_directories(Vocksel
        PRIVATE
        ${glm_SOURCE_DIR}                # GLM headers
        ${glfw_SOURCE_DIR}/include       # GLFW headers (optional, already green)
        include
)
