#include "game.hpp"
#include "mainmenu_scene.hpp"
#include "ingame_scene.hpp"
#include "demo_scene.hpp"


int main() {
    Game game;
    game.add_scene(std::make_shared<MainMenuScene>());
    game.add_scene(std::make_shared<InGameScene>());
    const auto id = game.add_scene(std::make_shared<DemoScene>());
    game.load_scene(id);
    game.run();

    return 0;
}
