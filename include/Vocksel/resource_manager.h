#ifndef RESOURCE__MANAGER_H
#define RESOURCE__MANAGER_H
#include <memory>
#include <string>

#include "texture_atlas.h"
#include "Vocksel/shader.h"

namespace Vocksel {
    class ResourceManager {
        public:
        ResourceManager();
        ~ResourceManager();

        void init();

        void loadShader(std::string name, const char* vertexPath, const char* fragmentPath);
        Shader& getShader(const std::string& name);

        void loadBlockAtlas(const std::string& path);
        TextureAtlas& getBlockAtlas();

        void cleanUp();

        // Prevent copying
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        private:
        std::unordered_map<std::string, std::unique_ptr<Shader>> shaders_;
        std::unique_ptr<TextureAtlas> blocks_atlas_;
        bool initialized_ = false;

    };
}

#endif //RESOURCE__MANAGER_H
