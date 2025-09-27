#include "Vocksel/Core/game.h"
#if DEBUG
#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"
#endif
#include "Vocksel/Core/engine_services.h"
#include "imgui.h"

Vocksel::Game::Game() {}

void Vocksel::Game::init() {
  world_ = std::make_unique<World>();
  world_->init();
  player_.init(*world_);

  player_.setPosition(world_->getSpawnPosition());

  EngineServices::input().bindKey(GLFW_KEY_R, [this] { world_->clearWorld(); });

  debugGUI();
}

void Vocksel::Game::debugGUI() {
  EngineServices::debugGUI().addPanel("Game", [this]() {
    static int model_index = 0;
    std::vector<std::string> model_names =
        EngineServices::resources().getNames<Model>();

    if (ImGui::BeginCombo("Model", model_names[model_index].c_str())) {
      for (int i = 0; i < model_names.size(); i++) {
        bool is_selected = (model_index == i);
        if (ImGui::Selectable(model_names[i].c_str(), is_selected)) {
          model_index = i;
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    static int texture_index = 0;
    std::vector<std::string> texture_names =
        EngineServices::resources().getNames<Texture>();

    if (ImGui::BeginCombo("Texture", texture_names[texture_index].c_str())) {
      for (int i = 0; i < texture_names.size(); i++) {
        bool is_selected = (texture_index == i);
        if (ImGui::Selectable(texture_names[i].c_str(), is_selected)) {
          texture_index = i;
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    static auto model_position = glm::vec3(0.0f);
    ImGui::InputFloat3("Position", glm::value_ptr(model_position));

    static auto model_scale = glm::vec3(0.0f);
    ImGui::InputFloat3("Scale", glm::value_ptr(model_scale));

    if (ImGui::Button("Spawn Model")) {
      model_instances_.push_back(ModelInstance::create(
          model_position, model_names[model_index].c_str(),
          texture_names[texture_index].c_str(), model_scale));
    }
  });
}

void Vocksel::Game::render() {
#if DEBUG
  ZoneScopedN("Game Render");
#endif
  auto& basic_shader = EngineServices::resources().get<Shader>("default");
  basic_shader.use();

  auto& camera = player_.getCamera();
  glm::mat4 view = camera.getViewMatrix();
  glm::mat4 projection =
      camera.getProjectionMatrix(EngineServices::aspect_ratio());

  basic_shader.setMat4("view", view);
  basic_shader.setMat4("projection", projection);

  for (auto& obj : model_instances_) {
    obj->render(basic_shader);
  }

  auto& world_shader = EngineServices::resources().get<Shader>("world");
  world_shader.use();
  world_shader.setMat4("view", view);
  world_shader.setMat4("projection", projection);
  world_->render(world_shader);
}

void Vocksel::Game::update(float delta_time) {
#if DEBUG
  ZoneScopedN("Game Update");
#endif
  world_->update(player_.getPosition());
  player_.update(delta_time);
}

void Vocksel::Game::handleMouseInput(float xoffset, float yoffset) {
  player_.handleMouseInput(xoffset, yoffset);
}
