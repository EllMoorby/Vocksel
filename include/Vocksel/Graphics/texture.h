#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>

namespace Vocksel {
class Texture {
 public:
  Texture();
  explicit Texture(const char* path);
  ~Texture();

  void createEmptyTexture(int width, int height);
  void loadTexture(const char* path);
  void bind();
  void unbind();
  void cleanUp();

  int getWidth() const { return width_; }
  int getHeight() const { return height_; }

  GLuint getID() const { return texture_id_; }

 private:
  int width_ = 0, height_ = 0;
  GLuint texture_id_ = 0;
};
}  // namespace Vocksel

#endif  // TEXTURE_H
