#ifndef MODEL_INSTANCE_H
#define MODEL_INSTANCE_H
#include <memory>

#include "Vocksel/Graphics/shader.h"

namespace Vocksel {
class ModelInstance {
 public:
  ModelInstance();
  ~ModelInstance() = default;

  static std::unique_ptr<ModelInstance> create(
      const glm::vec3& pos, const char* model_name, const char* texture_name,
      const glm::vec3& scale = glm::vec3(1.0f));

  void init(glm::vec3 pos, const char* model_name, const char* texture_name);
  void render(Shader& shader);

  void setPosition(const glm::vec3& pos) { position_ = pos; }
  void setScale(const glm::vec3& scale) { scale_ = scale; }
  void setRotation(float angle,
                   const glm::vec3& axis = glm::vec3(0.0f, 1.0f, 0.0f)) {
    rotation_angle_ = angle;
    rotation_axis_ = axis;
  }

  const glm::vec3& getPosition() const { return position_; }
  const glm::vec3& getScale() const { return scale_; }
  const glm::vec3& getRotationAxis() const { return rotation_axis_; }

  float getRotationAngle() const { return rotation_angle_; }

  float rotation_angle_ = 0.f;

 private:
  glm::vec3 position_;
  glm::vec3 scale_ = glm::vec3(1.f);
  glm::vec3 rotation_axis_ = glm::vec3(0.0f, 1.0f, 0.0f);

  std::string texture_name_;
  std::string model_name_;

  bool initialized_ = false;
};
}  // namespace Vocksel

#endif  // MODEL_INSTANCE_H
