//
// Created by hugo on 8/14/24.
//

#ifndef SCENE_HPP
#define SCENE_HPP

#include <SDL.h>

#include "game.hpp"

class IScene {
public:
    IScene() = default;

    virtual ~IScene() = default;

    virtual void on_load() = 0;

    virtual void on_unload() = 0;

    virtual void on_scene_enter() = 0;

    virtual void on_scene_exit() = 0;

    virtual void on_event(const SDL_Event &e) = 0;

    virtual void on_update(float dt) = 0;

    virtual void on_render(SDL_Renderer *renderer) = 0;

    void switch_to_scene(uint32_t id) {
        game_->load_scene(id);
    }

protected:
    Game *game_ = nullptr;

    friend class Game;
};

#endif //SCENE_HPP
