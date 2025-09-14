#ifndef DEBUG_GUI_H
#define DEBUG_GUI_H
#include <functional>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Vocksel {
class DebugGUI {
 public:
  ~DebugGUI();

  void init(GLFWwindow *window);
  void addPanel(const std::string &name,
                const std::function<void()> &callback) {
    panels_.emplace_back(name, callback);
  }
  void render();

 private:
  std::vector<std::pair<std::string, std::function<void()>>> panels_;
};
}  // namespace Vocksel

#endif  // DEBUG_GUI_H
