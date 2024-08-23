//
// Created by hugo on 8/14/24.
//

#ifndef MAINMENU_SCENE_HPP
#define MAINMENU_SCENE_HPP

#include "scene.hpp"

class MainMenuScene final : public IScene {
public:
    void on_load() override;

    void on_unload() override;

    void on_event(const SDL_Event &e) override;

    void on_update(float dt) override;

    void on_render(SDL_Renderer *renderer) override;

    void on_scene_enter() override;

    void on_scene_exit() override;
};


#endif //MAINMENU_SCENE_HPP
