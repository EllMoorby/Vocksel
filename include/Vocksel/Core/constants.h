#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <glm/glm.hpp>

namespace Vocksel::Constants {
    constexpr uint32_t SCREEN_WIDTH = 1920;
    constexpr uint32_t SCREEN_HEIGHT = 1080;
    constexpr uint32_t WORLD_SIZE = 10;
    constexpr uint32_t CHUNK_HEIGHT = 64;
    constexpr uint32_t CHUNK_SIZE = 16;
    constexpr float MOVEMENT_SPEED = 12.f;
    constexpr float CAMERA_SENS = .1f;
    constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
    constexpr float CREATURE_MAX_TURN = 100.f;
    constexpr float GRAVITY = -9.81f;

}

#endif //CONSTANTS_H
