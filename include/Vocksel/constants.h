#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <glm/glm.hpp>

namespace Vocksel::Constants {
    constexpr unsigned int SCREEN_WIDTH = 1920;
    constexpr unsigned int SCREEN_HEIGHT = 1080;
    constexpr unsigned int WORLD_SIZE = 10;
    constexpr unsigned int WORLD_HEIGHT = 32;
    constexpr unsigned int CHUNK_SIZE = 16;
    constexpr int CAMERA_SPEED = 5;
    constexpr float CAMERA_SENS = .1f;
    constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

}

#endif //CONSTANTS_H
