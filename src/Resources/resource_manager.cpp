#include "Vocksel/Resources/resource_manager.h"

#include "Vocksel/Core/engine_services.h"

Vocksel::ResourceManager::ResourceManager() {

}

void Vocksel::ResourceManager::init() {
    if (initialized_) return;

    blocks_atlas_ = std::make_unique<TextureAtlas>(160);

    loadBlockAtlas("assets/textures/blocks");
    loadShader("default","assets/shaders/Core/basic.vs.glsl", "assets/shaders/Core/basic.fs.glsl");
    loadShader("normals","assets/shaders/Core/normals.vs.glsl", "assets/shaders/Core/normals.fs.glsl");
    loadShader("line","assets/shaders/Core/Debug/line.vs.glsl", "assets/shaders/Core/Debug/line.fs.glsl");
    loadShader("world","assets/shaders/Core/World/world.vs.glsl", "assets/shaders/Core/World/world.fs.glsl");

    loadShader("marching_cubes", "assets/shaders/Core/World/marching_cubes.comp.glsl");


    loadTexture("default", "assets/textures/blocks/default.png");
    loadTexture("stone", "assets/textures/blocks/stone.png");
    loadTexture("grass", "assets/textures/blocks/grass.png");
    loadTexture("plank", "assets/textures/blocks/plank.png");
    loadTexture("wool", "assets/textures/blocks/wool.png");


    initDebug();

    initialized_ = true;
}

void Vocksel::ResourceManager::initDebug() {
    EngineServices::debugGUI().addPanel("Resource Manager", [this]() {
        if (ImGui::TreeNode("Shaders")) {
            if (ImGui::BeginTable("shader_table",3, ImGuiTableFlags_Borders)) {

                ImGui::TableSetupColumn("ID");
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("");
                ImGui::TableHeadersRow();

                for (auto& [name, shader] : shaders_) {
                    ImGui::TableNextRow();
                    ImGui::PushID(name.c_str());

                    ImGui::TableNextColumn();
                    ImGui::Text("%i", shader->ID_);

                    ImGui::TableNextColumn();
                    ImGui::Text("%s", name.c_str());

                    ImGui::TableNextColumn();
                    if (ImGui::Button("Refresh")) {
                        reloadShader(name);
                    }
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Textures")) {
            if (ImGui::BeginTable("texture_table",5, ImGuiTableFlags_Borders)) {

                ImGui::TableSetupColumn("ID");
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("Width");
                ImGui::TableSetupColumn("Height");
                ImGui::TableSetupColumn("Image");

                ImGui::TableHeadersRow();

                for (auto& [name, texture] : textures_) {
                    ImGui::TableNextRow();
                    ImGui::PushID(name.c_str());

                    ImGui::TableNextColumn();
                    ImGui::Text("%i", texture->getID());

                    ImGui::TableNextColumn();
                    ImGui::Text("%s", name.c_str());

                    ImGui::TableNextColumn();
                    ImGui::Text("%i", texture->getWidth());

                    ImGui::TableNextColumn();
                    ImGui::Text("%i", texture->getHeight());

                    ImGui::TableNextColumn();
                    ImGui::Image(texture->getID(), ImVec2(64, 64));

                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }

    });
}



void Vocksel::ResourceManager::loadShader(std::string name, const char *vertex_path, const char *fragment_path) {
    shaders_[name] = std::make_unique<Shader>(vertex_path, fragment_path);
}

void Vocksel::ResourceManager::loadShader(std::string name, const char *comp_path) {
    shaders_[name] = std::make_unique<Shader>(comp_path);
}

void Vocksel::ResourceManager::reloadShader(std::string name) {
    Shader* shader = shaders_[name].get();
    shader->reloadShader();
}



Vocksel::Shader& Vocksel::ResourceManager::getShader(const std::string &name) {
    if (!initialized_) throw std::runtime_error("ResourceManager not initialized");
    auto it = shaders_.find(name);
    if (it == shaders_.end()) {
        return *shaders_["default"];
    }
    return *(it->second);
}

void Vocksel::ResourceManager::loadTexture(std::string name, const char *path) {
    textures_[name] = std::make_unique<Texture>(path);
}


Vocksel::Texture& Vocksel::ResourceManager::getTexture(const std::string &name) {
    if (!initialized_) throw std::runtime_error("ResourceManager not initialized");
    auto it = textures_.find(name);
    if (it == textures_.end()) {
        return *textures_["default"];
    }
    return *(it->second);
}

std::vector<std::string> Vocksel::ResourceManager::getTextureNames() {
    if (!initialized_) throw std::runtime_error("ResourceManager not initialized");
    std::vector<std::string> names;
    for (auto& [name, texture] : textures_) {
        names.push_back(name);
    }
    return names;
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






