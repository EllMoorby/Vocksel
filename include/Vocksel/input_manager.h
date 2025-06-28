#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <functional>
#include <unordered_map>

#include "camera.h"
#include "GLFW/glfw3.h"

namespace Vocksel {
    class InputManager {
        public:
        void init(GLFWwindow* window);
        void bindKey(int key, std::function<void()> action);
        void bindKey(int key, std::function<void(float)> callback);
        void update(float delta_time);

        bool isKeyPressed(int key);
        bool isMouseFree();
        void setMouseMode(int mode);
        int getMouseMode();
        private:
        int mouse_mode_ = GLFW_CURSOR_DISABLED;
        GLFWwindow* window_ = nullptr;
        std::unordered_map<int, std::function<void(float)>> key_actions_;
        std::unordered_map<int, std::pair<bool, std::function<void()>>> key_release_actions_;
    };
}

#endif //INPUT_MANAGER_H
