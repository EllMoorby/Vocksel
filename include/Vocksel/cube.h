#ifndef CUBE_H
#define CUBE_H
#include <memory>
#include <glad/glad.h>
#include "Vocksel/model.h"
#include "static_mesh.h"
#include "model_manager.h"
#include "resource_manager.h"
#include "Vocksel/camera.h"
#include "Vocksel/shader.h"
#include "glm/vec3.hpp"


namespace Vocksel {
    class Cube {
    public:
        Cube(ModelManager& model_manager, ResourceManager& resource_manager);
        static std::unique_ptr<Cube> create(ModelManager& model_manager, ResourceManager& resource_manager, const glm::vec3& pos, const glm::vec3& col, const char* texture_name);

        void init(glm::vec3 pos, glm::vec3 col, const char* texture_name);
        void render(Shader& shader);
        static void cleanUp();
    private:
        static void initMesh(ModelManager& model_manager);
    private:
        glm::vec3 position_ = glm::vec3(0.f, .0f, .0f);
        glm::vec3 color_ = glm::vec3(0.0f, 0.0f, 0.0f);
        float rotation_angle_;
        bool initialized_ = false;
        ModelManager& model_manager_;
        ResourceManager& resource_manager_;
        std::string texture_name_;

        static const std::string MODEL_NAME;
    };
}

#endif //CUBE_H
