#ifndef CAMERA_H
#define CAMERA_H
#include "constants.h"
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Vocksel {
    class Camera {
    public:
        void moveForward(float delta_time);
        void moveBackward(float delta_time);
        void moveRight(float delta_time);
        void moveLeft(float delta_time);
        void moveUp(float delta_time);
        void moveDown(float delta_time);

        void recalculateVectors();

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix(float aspectRatio) const;
    public:
        glm::vec3 position_ = glm::vec3(0.f, 0.f, -3.f);
        glm::vec3 front_ = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 up_ = Constants::WORLD_UP;
        glm::vec3 right_ = glm::normalize(glm::cross(front_, up_));
        glm::vec3 flatfront_ = glm::normalize(glm::vec3(front_.x, 0.0f, front_.z));
    private:
        float fov_ = glm::radians(90.0f);
        float nearplane_ = 0.1f;
        float farplane_ = 400.0f;
    };
}

#endif //CAMERA_H
