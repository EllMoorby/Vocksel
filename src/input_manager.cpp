#include "Vocksel/input_manager.h"

#include <iostream>

void Vocksel::InputManager::init(GLFWwindow *window) {
    window_ = window;
}

void Vocksel::InputManager::bindKey(int key, std::function<void(float)> callback) {
    key_actions_[key] = callback;
}

void Vocksel::InputManager::bindKey(int key, std::function<void()> action) {
    key_actions_[key] = [action](float){ action(); };
}


void Vocksel::InputManager::update(float delta_time) {
    for (auto& [key, action] : key_actions_) {
        if (isKeyPressed(key)) {
            action(delta_time);
        }
    }
}

int Vocksel::InputManager::getMouseMode() {
    return mouse_mode_;
}

void Vocksel::InputManager::setMouseMode(int mode) {
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





