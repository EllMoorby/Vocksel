#include "Vocksel/Core/game.h"

#include "Vocksel/Core/engine_services.h"
#include "imgui.h"
#include "tracy/Tracy.hpp"

Vocksel::Game::Game() {

}

void Vocksel::Game::init() {
    world_ = std::make_unique<World>();
    world_->init();
    player_.init(*world_);

    player_.setPosition(world_->getSpawnPosition());

    model_instances_.push_back(ModelInstance::create(glm::vec3(-3.0f, .0f, 0.f), "teapot", "stone", glm::vec3(1.f)));
    entities_.push_back(std::make_unique<Creature>(glm::vec3(-3.f,8.f,0.f)));
}


void Vocksel::Game::render() {
    ZoneScoped;
    auto &basic_shader = EngineServices::resources().getShader("basic");
    basic_shader.use();

    auto& camera = player_.getCamera();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix(EngineServices::aspect_ratio());

    basic_shader.setMat4("view", view);
    basic_shader.setMat4("projection", projection);

    for (auto& obj : model_instances_) {
        obj->render(basic_shader);
    }

    for (auto& entity : entities_) {
        entity->render(basic_shader);
    }

    auto& world_shader = EngineServices::resources().getShader("world");
    world_shader.use();
    world_shader.setMat4("view", view);
    world_shader.setMat4("projection", projection);
    world_->render(world_shader);


    // Temporary Leg Viewing
    for (auto& creature : entities_) {
        auto* head = creature->getHeadSegment().get();

        for (auto& leg : head->getLegs()) {
            glm::vec3 currentBase = head->getPosition();
            for (auto& segment : leg.segments_) {
                EngineServices::debug().drawLine(currentBase, segment.getTipPosition(), glm::vec3(1, 0, 0), camera);
                currentBase = segment.getTipPosition();
            }
        }

        // Draw body segments legs
        for (auto& bodySegment : creature->getBodySegments()) {
            for (auto& leg : bodySegment->getLegs()) {
                glm::vec3 currentBase = bodySegment->getPosition();

                for (auto& segment : leg.segments_) {
                    EngineServices::debug().drawLine(currentBase, segment.getTipPosition(), glm::vec3(0, 1, 0),camera);
                    currentBase = segment.getTipPosition();
                }
            }
        }
    }



}

void Vocksel::Game::update(float delta_time) {
    ZoneScoped;
    player_.update(delta_time);
    for (auto& entity : entities_) {
        entity->update(delta_time);
    }
}

void Vocksel::Game::handleMouseInput(float xoffset, float yoffset) {
    player_.handleMouseInput(xoffset, yoffset);
}

void Vocksel::Game::renderDebugGUI() {
    ImGui::Begin("Debug");

    ImGui::Text("X: %.2f | Y: %.2f | Z: %.2f", player_.getPosition().x, player_.getPosition().y, player_.getPosition().z);
    ImGui::End();
}







