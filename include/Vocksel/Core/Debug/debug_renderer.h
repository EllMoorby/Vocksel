#ifndef DEBUG_RENDERER_H
#define DEBUG_RENDERER_H

#include <memory>

#include "Vocksel/Graphics/camera.h"
#include "Vocksel/Graphics/shader.h"

namespace Vocksel {
    class DebugRenderer {
    public:
        DebugRenderer();
        ~DebugRenderer();

        void init();

        void drawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color, Camera& camera);
    private:
        GLuint lineVAO, lineVBO = 0;
        std::shared_ptr<Shader> lineShader;
    };
}

#endif //DEBUG_RENDERER_H
