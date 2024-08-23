//
// Created by hugo on 8/16/24.
//

#include "ingame_scene.hpp"

#include <iostream>

void InGameScene::on_load() {
    std::cout << "InGameScene::on_load" << std::endl;
}

void InGameScene::on_unload() {
    std::cout << "InGameScene::on_unload" << std::endl;
}

void InGameScene::on_scene_enter() {
    std::cout << "InGameScene::on_scene_enter" << std::endl;
}

void InGameScene::on_scene_exit() {
    std::cout << "InGameScene::on_scene_exit" << std::endl;
}

void InGameScene::on_event(const SDL_Event &e) {
    // std::cout << "InGameScene::on_event" << std::endl;
}

void InGameScene::on_update(float dt) {
    // std::cout << "InGameScene::on_update" << std::endl;
}

void InGameScene::on_render(SDL_Renderer *renderer) {
    // std::cout << "InGameScene::on_render" << std::endl;
}
