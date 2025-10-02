#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <functional>
#include <unordered_map>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Vocksel/Graphics/camera.h"

namespace Vocksel {
class InputManager {
 public:
  void init(GLFWwindow* window);
  void bindKey(int key, std::function<void()> action);
  void bindKeyPress(int key, std::function<void()> action);

  void setMouseMode(int mode);
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

  glm::vec3 getWASDVector();
  glm::vec3 getArrowVector();

  bool isKeyPressed(int key);
  bool isMouseFree();

  int getMouseMode();

 private:
  int mouse_mode_ = GLFW_CURSOR_DISABLED;
  GLFWwindow* window_ = nullptr;
  static std::unordered_map<int, std::function<void()>> key_actions_;
  static std::unordered_map<int, std::function<void()>> key_press_actions_;
};
}  // namespace Vocksel

#endif  // INPUT_MANAGER_H
