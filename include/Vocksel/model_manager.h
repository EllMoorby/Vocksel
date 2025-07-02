#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H
#include <string>
#include <unordered_map>

#include "model.h"

namespace Vocksel {
    class ModelManager {
        public:
        ModelManager();
        ~ModelManager();

        void loadModel(const std::string& path, const std::string& name);
        bool createModelFromData(const std::string& name, const float* vertices, size_t vertexCount, const uint32_t* indices, size_t indexCount, int vertexStride);
        Model* getModel(const std::string& name);
        void cleanUp();
        private:
        std::unordered_map<std::string, std::unique_ptr<Model>> models_;

    };
}

#endif //MODEL_MANAGER_H
