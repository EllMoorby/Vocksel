#include "Vocksel/Core/input_manager.h"

#include <iostream>
#include <utility>

#include "Vocksel/Core/engine_services.h"

void Vocksel::InputManager::init(GLFWwindow* window) { window_ = window; }

void Vocksel::InputManager::bindKey(int key,
                                    std::function<void(float)> callback) {
  key_actions_[key] = std::move(callback);
}

void Vocksel::InputManager::bindKey(int key, std::function<void()> action) {
  key_actions_[key] = [action](float) { action(); };
}

glm::vec3 Vocksel::InputManager::getWASDVector() {
  glm::vec3 movement(0.0f);

  if (isKeyPressed(GLFW_KEY_W)) movement.z += 1.0f;
  if (isKeyPressed(GLFW_KEY_S)) movement.z -= 1.0f;
  if (isKeyPressed(GLFW_KEY_A)) movement.x -= 1.0f;
  if (isKeyPressed(GLFW_KEY_D)) movement.x += 1.0f;

  if (glm::length(movement) > 0.0f) {
    movement = glm::normalize(movement);
  }

  return movement;
}

glm::vec3 Vocksel::InputManager::getArrowVector() {
  glm::vec3 movement(0.0f);

  if (isKeyPressed(GLFW_KEY_UP)) movement.z += 1.0f;
  if (isKeyPressed(GLFW_KEY_DOWN)) movement.z -= 1.0f;
  if (isKeyPressed(GLFW_KEY_LEFT)) movement.x -= 1.0f;
  if (isKeyPressed(GLFW_KEY_RIGHT)) movement.x += 1.0f;

  if (glm::length(movement) > 0.0f) {
    movement = glm::normalize(movement);
  }

  return movement;
}

void Vocksel::InputManager::update() {
  for (auto& [key, action] : key_actions_) {
    if (isKeyPressed(key)) {
      action(EngineServices::deltaTime());
    }
  }
}

int Vocksel::InputManager::getMouseMode() { return mouse_mode_; }

void Vocksel::InputManager::setMouseMode(int mode) {
  // TODO: Add raw motion mode
  mouse_mode_ = mode;
  glfwSetInputMode(window_, GLFW_CURSOR, mode);
}

bool Vocksel::InputManager::isMouseFree() {
  if (mouse_mode_ == GLFW_CURSOR_NORMAL) {
    return true;
  }
  return false;
}

bool Vocksel::InputManager::isKeyPressed(int key) {
  return glfwGetKey(window_, key) == GLFW_PRESS;
}
