//
// Created by Max Campbell on 7/27/22.
//

#ifndef FLAME_ENGINE_CORE_H
#define FLAME_ENGINE_CORE_H
#pragma once

#include "graphics.h"
#include "consts.h"
#include <vector>
#include <miniaudio.h>

class Flame {
public:
    RenWindowHandler ren_win;
    std::vector<TextData*> draw_text;
    SDL_Event event_handler;
    bool quit = false;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    Flame(int window_x_size, int window_y_size,char* title);
    void cycle(void);
    bool is_key_pressed(Uint8 key);
    void DrawText();
};

class GameObject {
private:
    bool is_rect_colliding(GameObject* gameObject);
    bool is_circle_colliding(GameObject* gameObject);
    bool is_circle_and_rect_colliding(GameObject* gameObject);
public:
    GameObjectGraphics* graphics;
    Flame* flame;
    std::vector<std::function<void(GameObject*,Flame*)>> functions;
    void assign_graphics(GameObjectGraphics* set_graphics,Flame* set_flame);
    void add_behavior(std::function<void(GameObject*,Flame*)> behavior);
    bool is_colliding(GameObject* gameObject);
    void render();
    void update();
};

void Shutdown(Flame* flame);

#endif //FLAME_ENGINE_CORE_H
