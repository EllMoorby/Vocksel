#ifndef CUBE_H
#define CUBE_H
#include <glad/glad.h>
#include "camera.h"
#include "glm/vec3.hpp"


namespace Vocksel {
    class Cube {
    public:
        Cube(glm::vec3 pos);
        ~Cube();
        void update(float deltaTime);
        void render(unsigned int shaderProgram, const Camera& camera);

    private:
        glm::vec3 position_ = glm::vec3(0.f, .0f, .0f);
        float rotation_angle_;
        GLuint VAO_, VBO_, EBO_;

        // Cube vertices
        static const float vertices[24];
        static const unsigned int indices[36];

    };
}

#endif //CUBE_H
