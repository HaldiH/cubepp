//
// Created by hugo on 8/16/24.
//

#ifndef INGAME_SCENE_HPP
#define INGAME_SCENE_HPP

#include <SDL.h>

#include "scene.hpp"

class InGameScene : public IScene {
public:
    void on_load() override;

    void on_unload() override;

    void on_scene_enter() override;

    void on_scene_exit() override;

    void on_event(const SDL_Event &e) override;

    void on_update(float dt) override;

    void on_render(SDL_Renderer *renderer) override;
};


#endif //INGAME_SCENE_HPP
