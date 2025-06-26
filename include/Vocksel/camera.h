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
        void moveForward(float speed);
        void moveBackward(float speed);
        void moveRight(float speed);
        void moveLeft(float speed);
        void moveUp(float speed);
        void moveDown(float speed);


        void recalculateVectors();
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix(float aspectRatio) const;

        glm::vec3 position_ = glm::vec3(0.f, 0.f, -3.f);
        glm::vec3 front_ = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
        glm::vec3 up_ = Constants::WORLD_UP;
        glm::vec3 right_ = glm::normalize(glm::cross(front_, up_));

    private:
        float fov_ = glm::radians(45.0f);
        float nearplane_ = 0.1f;
        float farplane_ = 100.0f;
    };
}

#endif //CAMERA_H
