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
  void update();
  void bindKey(int key, std::function<void()> action);
  void bindKey(int key, std::function<void(float)> callback);
  void setMouseMode(int mode);

  glm::vec3 getWASDVector();
  glm::vec3 getArrowVector();

  bool isKeyPressed(int key);
  bool isMouseFree();

  int getMouseMode();

 private:
  int mouse_mode_ = GLFW_CURSOR_DISABLED;
  GLFWwindow* window_ = nullptr;
  std::unordered_map<int, std::function<void(float)>> key_actions_;
  std::unordered_map<int, std::pair<bool, std::function<void()>>>
      key_release_actions_;
};
}  // namespace Vocksel

#endif  // INPUT_MANAGER_H
