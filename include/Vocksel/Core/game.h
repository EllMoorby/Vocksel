#ifndef GAME_H
#define GAME_H
#include "Vocksel/Entities/player.h"
#include "Vocksel/Entities/Creature/creature.h"
#include "Vocksel/Graphics/Mesh/model_instance.h"

namespace Vocksel {
    class Game {
        public:
        Game();
        ~Game() = default;

        void init();
        void update(float delta_time);
        void render();

        void handleMouseInput(float xoffset, float yoffset);

        void setClearColor(float r, float g, float b, float a) {clear_color_ = glm::vec4(r, g, b, a);}
        void setClearColor(glm::vec4 color) {clear_color_ = color;}

        glm::vec4 getClearColor() {return clear_color_;}

        Player& getPlayer() {return player_;}

        private:
        Player player_;
        std::unique_ptr<World> world_;
        std::vector<std::unique_ptr<ModelInstance>> model_instances_;
        std::vector<std::unique_ptr<Creature>> entities_;
        glm::vec4 clear_color_ = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
    };
}

#endif //GAME_H
