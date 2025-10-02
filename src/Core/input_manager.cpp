#include "Vocksel/Core/input_manager.h"

#include <iostream>
#include <utility>

#include "Vocksel/Core/engine_services.h"

std::unordered_map<int, std::function<void()>> Vocksel::InputManager::key_press_actions_;
std::unordered_map<int, std::function<void()>> Vocksel::InputManager::key_actions_;

void Vocksel::InputManager::init(GLFWwindow* window){
  window_ = window;
  glfwSetKeyCallback(window_, keyCallback);
}

void Vocksel::InputManager::bindKey(int key, std::function<void()> action) {
  key_actions_[key] = [action]() { action(); };
}

void Vocksel::InputManager::bindKeyPress(int key, std::function<void()> action) {
  key_press_actions_[key] = [action]() { action(); };
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

int Vocksel::InputManager::getMouseMode() { return mouse_mode_; }

void Vocksel::InputManager::setMouseMode(int mode) {
  mouse_mode_ = mode;
  glfwSetInputMode(window_, GLFW_CURSOR, mode);
}

void Vocksel::InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    auto it = key_press_actions_.find(key);
    if (it != key_press_actions_.end()) {
      it->second();
    }
  }

  auto normal_it = key_actions_.find(key);
  if (normal_it != key_actions_.end()) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      normal_it->second();
    }
  }

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
