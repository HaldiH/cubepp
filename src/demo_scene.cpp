//
// Created by hugo on 8/16/24.
//

#include "demo_scene.hpp"

#include <iostream>
#include <numeric>
#include <SDL_ttf.h>

struct FrameData {
    VkCommandPool _command_pool;
    VkCommandBuffer _main_command_buffer;
};

void DemoScene::on_load() {
    TTF_Init();
    sans_font_ = TTF_OpenFont("../assets/fonts/OpenSans-Regular.ttf", 24);
    if (!sans_font_) {
        printf("Failed to load font: %s\n", TTF_GetError());
        exit(1);
    }
}

void DemoScene::on_unload() {
}

void DemoScene::on_scene_enter() {
}

void DemoScene::on_scene_exit() {
}

void DemoScene::on_event(const SDL_Event &e) {
}

void DemoScene::on_update(float dt) {
    fps_ = 1.0f / dt;
    fps_array_[fps_counter_++] = fps_;
    if (fps_counter_ == 1024) {
        fps_avg_ = std::accumulate(fps_array_, fps_array_ + 1024, 0);
        fps_counter_ = 0;
        fps_avg_ /= 1024.0;
    }
}

void DemoScene::on_render(SDL_Renderer *renderer) {
    // SDL_SetRenderDrawColor(renderer, std::rand() % 256, std::rand() % 256, std::rand() % 256, 255);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    auto *msg_surface = TTF_RenderText_Solid(sans_font_, std::format("FPS: {}", fps_avg_).c_str(),
                                             SDL_Color{255, 0, 0});
    auto *msg = SDL_CreateTextureFromSurface(renderer, msg_surface);
    const SDL_Rect msg_rect{0, 0, 100, 40};
    SDL_RenderCopy(renderer, msg, nullptr, &msg_rect);

    SDL_DestroyTexture(msg);
    SDL_FreeSurface(msg_surface);

    SDL_RenderPresent(renderer);
}
