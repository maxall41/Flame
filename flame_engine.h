//
// Created by Max Campbell on 7/27/22.
//

#ifndef FLAME_ENGINE_FLAME_ENGINE_H
#define FLAME_ENGINE_FLAME_ENGINE_H
#pragma once

class Flame {
public:
    RenWindowHandler ren_win;
    SDL_Event event_handler;
    bool quit = false;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    Flame(int window_x_size, int window_y_size,char* title);
    void cycle(void);
    bool is_key_pressed(Uint8 key);
};

class GameObject {
public:
    GameObjectGraphics* graphics;
    Flame* flame;
    std::vector<std::function<void(GameObject*,Flame*)>> functions;
    void assign_graphics(GameObjectGraphics* set_graphics,Flame* set_flame);
    void add_behavior(std::function<void(GameObject*,Flame*)> behavior);
    void render();
    void update();
};

void Shutdown(RenWindowHandler ren_win)

#endif //FLAME_ENGINE_FLAME_ENGINE_H
