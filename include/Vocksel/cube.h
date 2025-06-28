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

        void init(glm::vec3 pos, glm::vec3 col);
        void initTexture(const char* texture);
        static Cube create(const glm::vec3& pos, const glm::vec3& col);
        void render(Shader& shader);
        static void cleanUpMesh();
    public:
        static GLuint texture_id_;
    private:
        static void initMesh();
    private:
        glm::vec3 position_ = glm::vec3(0.f, .0f, .0f);
        glm::vec3 color_ = glm::vec3(0.0f, 0.0f, 0.0f);
        float rotation_angle_;
        bool initialized_ = false;
        static std::unique_ptr<StaticMesh> mesh_;

        // Cube vertices
        static const float vertices_[];
        static const unsigned int indices_[36];
    };
}

#endif //CUBE_H
