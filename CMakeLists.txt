cmake_minimum_required(VERSION 3.31)
project(Vocksel LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
)

# Fetch and configure GLM (OpenGL Mathematics)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
)

# Fetch ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.0
)
FetchContent_MakeAvailable(glfw glm imgui)

add_library(glad STATIC third_party/glad/glad.c)
target_include_directories(glad PUBLIC include)

add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)
target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_include_directories(imgui PRIVATE ${glfw_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)

add_executable(Vocksel
        src/main.cpp
        src/cube.cpp
        include/Vocksel/cube.h
        src/shader.cpp
        include/Vocksel/shader.h
        include/Vocksel/constants.h
        include/Vocksel/camera.h
        src/camera.cpp
        include/Vocksel/application.h
        src/application.cpp
        include/Vocksel/static_mesh.h
        src/static_mesh.cpp
        include/Vocksel/chunk.h
        src/chunk.cpp
        include/Vocksel/texture_atlas.h
        src/texture_atlas.cpp
        include/Vocksel/world.h
        src/world.cpp
        include/Vocksel/input_manager.h
        src/input_manager.cpp
        include/Vocksel/resource_manager.h
        src/resource_manager.cpp
        src/model_manager.cpp
        include/Vocksel/model_manager.h
        src/model.cpp
        include/Vocksel/model.h
        include/Vocksel/texture.h
        src/texture.cpp
        include/Vocksel/player.h
        src/player.cpp
)

# Copy assets directory to build
add_custom_command(TARGET Vocksel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

# Set debug mode.
target_compile_definitions(Vocksel PRIVATE Debug)


target_link_libraries(Vocksel
        PRIVATE
        OpenGL::GL
        glad
        glfw
        imgui
)
# Include directories
target_include_directories(Vocksel
        PRIVATE
        ${glm_SOURCE_DIR}
        ${glfw_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR}/include
        include
)
