#include "Vocksel/resource_manager.h"

Vocksel::ResourceManager::ResourceManager() {

}

void Vocksel::ResourceManager::init() {

    if (initialized_) return;
    blocks_atlas_ = std::make_unique<TextureAtlas>(160);

    loadBlockAtlas("assets/textures");
    loadShader("basic","assets/shaders/core/basic.vs.glsl", "assets/shaders/core/basic.fs.glsl");
    loadShader("normals","assets/shaders/core/normals.vs.glsl", "assets/shaders/core/normals.fs.glsl");


    initialized_ = true;
}


void Vocksel::ResourceManager::loadShader(std::string name, const char *vertexPath, const char *fragmentPath) {
    shaders_[name] = std::make_unique<Shader>(vertexPath, fragmentPath);
}

Vocksel::Shader& Vocksel::ResourceManager::getShader(const std::string &name) {
    if (!initialized_) throw std::runtime_error("ResourceManager not initialized");
    return *shaders_[name];
}

void Vocksel::ResourceManager::loadBlockAtlas(const std::string &path) {
    blocks_atlas_->loadFromFolder(path);
}

Vocksel::TextureAtlas& Vocksel::ResourceManager::getBlockAtlas() {
    if (!initialized_ || !blocks_atlas_)
        throw std::runtime_error("ResourceManager not initialized or TextureAtlas missing");
    return *blocks_atlas_;
}

void Vocksel::ResourceManager::cleanUp() {
    shaders_.clear();
    blocks_atlas_->cleanUp();
}

Vocksel::ResourceManager::~ResourceManager() {
    cleanUp();
}






