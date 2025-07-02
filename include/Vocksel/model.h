#ifndef MODEL_H
#define MODEL_H
#include <memory>
#include <string>

#include "Vocksel/shader.h"
#include "Vocksel/static_mesh.h"

namespace Vocksel {
    class Model {
        public:
        Model();
        ~Model();

        bool loadFromFile(std::string path);
        void addMesh(std::unique_ptr<StaticMesh> mesh);

        void render(Shader& shader);
        void setTransformMatrix(const glm::mat4& transform);

        void cleanUp();
        private:
        std::vector<std::unique_ptr<StaticMesh>> meshes_;
        glm::mat4 transform_ = glm::mat4(1.0f);
    };
}

#endif //MODEL_H
