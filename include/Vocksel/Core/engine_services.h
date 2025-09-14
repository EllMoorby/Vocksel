#ifndef ENGINE_SERVICES_H
#define ENGINE_SERVICES_H
#include "Debug/debug_gui.h"
#include "Debug/debug_renderer.h"
#include "Vocksel/Core/input_manager.h"
#include "Vocksel/Resources/resource_manager.h"

namespace Vocksel {
class EngineServices {
 public:
  ~EngineServices();
  static void init(GLFWwindow* window);

  static ResourceManager& resources() {
    if (!resource_manager_)
      throw std::runtime_error("ResourceManager not initialized");
    return *resource_manager_;
  }
  static InputManager& input() {
    if (!input_manager_)
      throw std::runtime_error("InputManager not initialized");
    return *input_manager_;
  }

  static DebugGUI& debugGUI() {
    if (!debug_gui_) throw std::runtime_error("DebguGUI not initialized");
    return *debug_gui_;
  }

  static DebugRenderer& debug() {
    if (!debug_renderer_)
      throw std::runtime_error("DebugRenderer not initialized");
    return *debug_renderer_;
  }

  static float deltaTime() {
    if (!Frame::initialized_)
      throw std::runtime_error("Frame data not initialized");
    return Frame::delta_time_;
  }

  static float aspect_ratio() {
    if (!Frame::initialized_)
      throw std::runtime_error("Frame data not initialized");
    return Frame::aspect_ratio_;
  }

  static void updateFrameData(float dt, float aspect);

  static void cleanUp() {
    input_manager_.reset();
    resource_manager_.reset();
    debug_renderer_.reset();
    debug_gui_.reset();
  }

 private:
  static std::unique_ptr<ResourceManager> resource_manager_;
  static std::unique_ptr<InputManager> input_manager_;
  static std::unique_ptr<DebugRenderer> debug_renderer_;
  static std::unique_ptr<DebugGUI> debug_gui_;

  struct Frame {
    inline static float delta_time_ = 1.0f / 60.0f;
    inline static float aspect_ratio_ = 16.0f / 9.0f;
    inline static bool initialized_ = false;
  };
};
}  // namespace Vocksel

#endif  // ENGINE_SERVICES_H
