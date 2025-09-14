#include "Vocksel/Core/Debug/debug_renderer.h"

#include "Vocksel/Core/engine_services.h"

Vocksel::DebugRenderer::DebugRenderer() = default;

void Vocksel::DebugRenderer::init() {
  glGenVertexArrays(1, &lineVAO);
  glGenBuffers(1, &lineVBO);
  glBindVertexArray(lineVAO);
  glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
  glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

void Vocksel::DebugRenderer::drawLine(const glm::vec3 &start,
                                      const glm::vec3 &end,
                                      const glm::vec3 &color, Camera &camera) {
  float vertices[6] = {start.x, start.y, start.z, end.x, end.y, end.z};

  auto &lineShader = EngineServices::resources().getShader("line");
  lineShader.use();

  lineShader.setMat4("view", camera.getViewMatrix());
  lineShader.setMat4(
      "projection", camera.getProjectionMatrix(EngineServices::aspect_ratio()));
  lineShader.setVec3("color", color);

  glBindVertexArray(lineVAO);
  glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  glLineWidth(2.0f);
  glDrawArrays(GL_LINES, 0, 2);
  glBindVertexArray(0);
}

Vocksel::DebugRenderer::~DebugRenderer() {
  glDeleteVertexArrays(1, &lineVAO);
  glDeleteBuffers(1, &lineVBO);
}
