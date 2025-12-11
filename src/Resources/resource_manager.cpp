#include "Vocksel/Resources/resource_manager.h"

#include <filesystem>

#include "Vocksel/Core/engine_services.h"

Vocksel::ResourceManager::ResourceManager() {}

void Vocksel::ResourceManager::init() {
  if (initialized_) return;

  load<TextureAtlas>("blocks", 160, "assets/textures/blocks");

  load<Shader>("default", "assets/shaders/Core/basic.vs.glsl",
               "assets/shaders/Core/basic.fs.glsl");
  load<Shader>("normals", "assets/shaders/Core/normals.vs.glsl",
               "assets/shaders/Core/normals.fs.glsl");
  load<Shader>("line", "assets/shaders/Core/Debug/line.vs.glsl",
               "assets/shaders/Core/Debug/line.fs.glsl");
  load<Shader>("world", "assets/shaders/Core/World/world.vs.glsl",
               "assets/shaders/Core/World/world.fs.glsl");

  load<Shader>("marching_cubes",
               "assets/shaders/Core/World/marching_cubes.comp.glsl");

  load<Texture>("default", "assets/textures/blocks/default.png");
  load<Texture>("stone", "assets/textures/blocks/stone.png");
  load<Texture>("grass", "assets/textures/blocks/grass.png");
  load<Texture>("plank", "assets/textures/blocks/plank.png");
  load<Texture>("wool", "assets/textures/blocks/wool.png");

  load<Model>("cube", "assets/models/cube.obj");
  load<Model>("sphere", "assets/models/sphere.obj");
  load<Model>("teapot", "assets/models/teapot.obj");

  initDebug();

  initialized_ = true;
}

void Vocksel::ResourceManager::initDebug() {
  EngineServices::debugGUI().addPanel("Resource Manager", [this]() {
    if (ImGui::TreeNode("Shaders")) {
      if (ImGui::BeginTable("shader_table", 3, ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("");
        ImGui::TableHeadersRow();

        auto& shader_map = getResourceMap<Shader>();
        for (auto& [name, shader] : shader_map) {
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
      if (ImGui::BeginTable("texture_table", 5, ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Width");
        ImGui::TableSetupColumn("Height");
        ImGui::TableSetupColumn("Image");

        ImGui::TableHeadersRow();

        auto& texture_map = getResourceMap<Texture>();
        for (auto& [name, texture] : texture_map) {
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

void Vocksel::ResourceManager::reloadShader(std::string name) {
  auto& shader = get<Shader>(name);
  shader.reloadShader();
}

void Vocksel::ResourceManager::cleanUp() { resource_maps_.clear(); }

Vocksel::ResourceManager::~ResourceManager() { cleanUp(); }
