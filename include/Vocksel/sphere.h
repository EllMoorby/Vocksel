#ifndef SPHERE_H
#define SPHERE_H
#include "model_manager.h"
#include "resource_manager.h"

namespace Vocksel {
    class Sphere {
    public:
        Sphere(ModelManager& model_manager, ResourceManager& resource_manager);
        static std::unique_ptr<Sphere> create(ModelManager& model_manager, ResourceManager& resource_manager, const glm::vec3& pos, const float radius, const char* texture_name);

        void init(glm::vec3 pos, float radius, const char* texture_name);
        void render(Shader& shader);
        void setPosition(const glm::vec3& pos) {position_ = pos;};
        static void cleanUp();

    private:
        static void initMesh(ModelManager& model_manager);
    private:
        glm::vec3 position_ = glm::vec3(0.f, .0f, .0f);
        float radius_ = 0;
        bool initialized_ = false;
        ModelManager& model_manager_;
        ResourceManager& resource_manager_;
        std::string texture_name_;

        static const std::string MODEL_NAME;
    };
}


#endif //SPHERE_H
