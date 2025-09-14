#include "Vocksel/Graphics/texture_3d.h"

#include <iostream>
#include <ostream>

Vocksel::Texture3D::Texture3D(const float *data, glm::ivec3 size,
                              uint32_t format)
    : size_(size), format_(format) {
  createFromData(data, size_, format_);
}

void Vocksel::Texture3D::createIfNeeded(const float *data, glm::ivec3 size,
                                        uint32_t format) {
  if (texture_id_ != 0) {
    return;
  }
  createFromData(data, size, format);
}

void Vocksel::Texture3D::createFromData(const float *data, glm::ivec3 size,
                                        uint32_t format) {
  if (texture_id_ != 0) {
    return;
  }

  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_3D, texture_id_);

  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  size_ = size;
  format_ = format;

  glTexStorage3D(GL_TEXTURE_3D, 1, format, size.x, size.y, size.z);
  if (data) {
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0,  // offset
                    size.x, size.y, size.z, GL_RED, GL_FLOAT, data);
  }
}

void Vocksel::Texture3D::bind() {
  if (texture_id_ == 0) {
    return;
  }
  glBindTexture(GL_TEXTURE_3D, texture_id_);
}

void Vocksel::Texture3D::unbind() { glBindTexture(GL_TEXTURE_3D, 0); }

void Vocksel::Texture3D::cleanUp() {
  if (texture_id_ != 0) {
    glDeleteTextures(1, &texture_id_);
    texture_id_ = 0;
    size_ = glm::ivec3(0);
  }
}

Vocksel::Texture3D::~Texture3D() { cleanUp(); }
