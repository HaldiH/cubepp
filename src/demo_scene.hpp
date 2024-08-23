//
// Created by hugo on 8/16/24.
//

#ifndef DEMO_SCENE_HPP
#define DEMO_SCENE_HPP

#include <SDL_ttf.h>

#include "scene.hpp"

class DemoScene : public IScene {
public:
    void on_load() override;

    void on_unload() override;

    void on_scene_enter() override;

    void on_scene_exit() override;

    void on_event(const SDL_Event &e) override;

    void on_update(float dt) override;

    void on_render(SDL_Renderer *renderer) override;

private:
    TTF_Font *sans_font_ = nullptr;

    int fps_ = 0;
    int fps_array_[1024];
    int fps_counter_ = 0;
    int fps_avg_ = 0;


};

#endif //DEMO_SCENE_HPP
