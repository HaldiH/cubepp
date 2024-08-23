//
// Created by hugo on 8/14/24.
//

#include "mainmenu_scene.hpp"

#include <iostream>

void MainMenuScene::on_load() {
    std::cout << "Main scene loaded" << std::endl;
}

void MainMenuScene::on_unload() {
    std::cout << "Main scene unloaded" << std::endl;
}

void MainMenuScene::on_event(const SDL_Event &e) {
}

void MainMenuScene::on_update(float dt) {
    switch_to_scene(1);
}

void MainMenuScene::on_render(SDL_Renderer *renderer) {
}

void MainMenuScene::on_scene_enter() {
}

void MainMenuScene::on_scene_exit() {
}
