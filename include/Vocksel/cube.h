#ifndef CUBE_H
#define CUBE_H
#include <glad/glad.h>
#include "Vocksel/camera.h"
#include "Vocksel/shader.h"
#include "glm/vec3.hpp"


namespace Vocksel {
    class Cube {
    public:
        Cube(glm::vec3 pos);
        Cube();
        ~Cube();

        Cube(const Cube&) = delete;
        Cube& operator=(const Cube&) = delete;

        Cube(Cube&&) noexcept;
        Cube& operator=(Cube&&) noexcept;

        void init(glm::vec3 pos);
        void update(float deltaTime);
        void render(Shader& shader, const Camera& camera);

    private:
        glm::vec3 position_ = glm::vec3(0.f, .0f, .0f);
        float rotation_angle_;
        GLuint VAO_, VBO_, EBO_;
        bool initialized_ = false;

        // Cube vertices
        static const float vertices[24];
        static const unsigned int indices[36];

    };
}

#endif //CUBE_H
