#ifndef CUBE_H
#define CUBE_H
#include <memory>
#include <glad/glad.h>

#include "static_mesh.h"
#include "Vocksel/camera.h"
#include "Vocksel/shader.h"
#include "glm/vec3.hpp"


namespace Vocksel {
    class Cube {
    public:
        Cube();
        static Cube create(const glm::vec3& pos, const glm::vec3& col);

        void init(glm::vec3 pos, glm::vec3 col);
        void render(Shader& shader, const Camera& camera);

        static void initMesh();
        static void cleanUpMesh();



    private:
        glm::vec3 position_ = glm::vec3(0.f, .0f, .0f);
        glm::vec3 color_ = glm::vec3(0.0f, 0.0f, 0.0f);
        float rotation_angle_;
        bool initialized_ = false;

        static std::unique_ptr<StaticMesh> mesh_;

        // Cube vertices
        static const float vertices[24];
        static const unsigned int indices[36];

    };
}

#endif //CUBE_H
