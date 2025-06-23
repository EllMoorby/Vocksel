#ifndef CAMERA_H
#define CAMERA_H
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Vocksel {
    class Camera {
    public:
        void moveForward(float speed);
        void moveBackward(float speed);

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix(float aspectRatio) const;

        glm::vec3 position_ = glm::vec3(0.f, 0.f, 3.f);
        glm::vec3 front_ = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up_    = glm::vec3(0.0f, 1.0f,  0.0f);

    private:
        float fov_ = glm::radians(45.0f);
        float nearplane_ = 0.1f;
        float farplane_ = 100.0f;
    };
}

#endif //CAMERA_H
