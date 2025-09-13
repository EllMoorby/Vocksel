cmake_minimum_required(VERSION 3.31)
project(Vocksel LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib" ${CMAKE_FIND_LIBRARY_SUFFIXES})
endif()



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

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Fetch Tracy
    FetchContent_Declare(
            tracy
            GIT_REPOSITORY https://github.com/wolfpld/tracy.git
    )
    FetchContent_MakeAvailable(tracy)
endif ()

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

if (CMAKE_BUILD_TYPE STREQUAL "Debug")

    # Add Tracy client library

    add_library(tracy_client STATIC
            ${tracy_SOURCE_DIR}/public/TracyClient.cpp
    )
    target_include_directories(tracy_client PUBLIC
            ${tracy_SOURCE_DIR}/public
    )
    target_compile_definitions(tracy_client PUBLIC
            TRACY_ENABLE
            TRACY_ON_DEMAND
    )
endif ()

find_package(OpenGL REQUIRED)

add_executable(Vocksel
        src/main.cpp
        src/Graphics/shader.cpp
        include/Vocksel/Graphics/shader.h
        include/Vocksel/Core/constants.h
        include/Vocksel/Graphics/camera.h
        src/Graphics/camera.cpp
        include/Vocksel/Core/application.h
        src/Core/application.cpp
        include/Vocksel/Graphics/Mesh/static_mesh.h
        src/Graphics/Mesh/static_mesh.cpp
        include/Vocksel/World/chunk.h
        src/World/chunk.cpp
        include/Vocksel/Graphics/texture_atlas.h
        src/Resources/texture_atlas.cpp
        include/Vocksel/World/world.h
        src/World/world.cpp
        include/Vocksel/Core/input_manager.h
        src/Core/input_manager.cpp
        include/Vocksel/Resources/resource_manager.h
        src/Resources/resource_manager.cpp
        src/Resources/model_manager.cpp
        include/Vocksel/Resources/model_manager.h
        src/Graphics/Mesh/model.cpp
        include/Vocksel/Graphics/Mesh/model.h
        include/Vocksel/Graphics/texture.h
        src/Graphics/texture.cpp
        include/Vocksel/Entities/player.h
        src/Entities/player.cpp
        include/Vocksel/Entities/Creature/creature.h
        include/Vocksel/Entities/Creature/creature_segment.h
        src/Entities/Creature/creature_segment.cpp
        src/Entities/Creature/creature.cpp
        include/Vocksel/Core/engine_services.h
        src/Core/engine_services.cpp
        include/Vocksel/Graphics/Mesh/model_instance.h
        src/Graphics/Mesh/model_instance.cpp
        include/Vocksel/Entities/Creature/leg_segment.h
        include/Vocksel/Entities/Creature/leg_chain.h
        src/Entities/Creature/leg_chain.cpp
        src/Entities/Creature/leg_segment.cpp
        include/Vocksel/Core/Debug/debug_renderer.h
        src/Core/Debug/debug_renderer.cpp
        include/Vocksel/Entities/Debug/leg_creature.h
        src/Entities/Creature/Debug/leg_creature.cpp
        include/Vocksel/Core/game.h
        src/Core/game.cpp
        include/Vocksel/World/density_field.h
        include/Vocksel/Graphics/texture_3d.h
        include/Vocksel/Graphics/Mesh/compute_mesh.h
        include/Vocksel/World/marching_cubes.h
        src/Graphics/texture_3d.cpp
        src/World/density_field.cpp
        src/Graphics/Mesh/compute_mesh.cpp
        src/World/marching_cubes.cpp
        include/Vocksel/World/mc_lookup.h
        include/Vocksel/Core/Debug/debug_gui.h
        src/Core/Debug/debug_gui.cpp
)

# Copy assets directory to build
add_custom_command(TARGET Vocksel POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Vocksel PRIVATE DEBUG=1)
else ()
    target_compile_definitions(Vocksel PRIVATE DEBUG=0)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(Vocksel
            PRIVATE
            OpenGL::GL
            glad
            glfw
            imgui
            tracy_client
    )
else ()
    target_link_libraries(Vocksel
            PRIVATE
            OpenGL::GL
            glad
            glfw
            imgui
    )
endif ()

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(tracy_client PRIVATE ws2_32 dbghelp)
endif()

# Include directories
target_include_directories(Vocksel
        PRIVATE
        ${glm_SOURCE_DIR}
        ${glfw_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR}/include
        include
)

if(TRACY_ENABLE AND OPENGL_FOUND AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(tracy_client PUBLIC TRACY_OPENGL)
    target_link_libraries(tracy_client PRIVATE OpenGL::GL)
endif()
