#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <functional>
#include <vector>
#include <iostream>
#include "graphics.h"
#include "flame_engine.h"



void Shutdown(RenWindowHandler ren_win) {
    SDL_DestroyRenderer(ren_win.ren);
    SDL_Quit();
}

Flame::Flame(int window_x_size, int window_y_size,char* title) {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);

    SDL_Window* win = SDL_CreateWindow(title,0,0,window_x_size,window_y_size,0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(ren);
    ren_win = {ren,win};
}

bool Flame::is_key_pressed(Uint8 key) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    return state[key];
}

void Flame::cycle(void) {
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();

    deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() ) * 0.001;
    //Handle events on queue
    while( SDL_PollEvent( &event_handler ) != 0 ) // poll for event
    {
        switch (event_handler.type) {
            // exit if the window is closed
            case SDL_QUIT:
                quit = true;
                break;
            default:
                break;
        }
    }
}

void GameObject::assign_graphics(GameObjectGraphics* set_graphics,Flame* set_flame) {
    graphics = set_graphics;
    flame = set_flame;
}

void GameObject::add_behavior(std::function<void(GameObject*,Flame*)> behavior) {
    functions.push_back(behavior);
}

void GameObject::render() {
    switch(graphics->type) {
        case GraphicsTypes::circle_filled:
            DrawFilledCircle(flame->ren_win.ren,graphics->x_pos,graphics->y_pos,graphics->radius,graphics->color);
            break;
        case GraphicsTypes::circle_outline:
            DrawCircleOutline(flame->ren_win.ren,graphics->x_pos,graphics->y_pos,graphics->radius,graphics->color);
            break;
        case GraphicsTypes::rectangle_outline:
            DrawOutlineRectangle(graphics->x_pos,graphics->y_pos,graphics->width,graphics->height,flame->ren_win.ren,graphics->color);
            break;
        case GraphicsTypes::rectangle_filled:
            DrawFilledRectangle(graphics->x_pos,graphics->y_pos,graphics->width,graphics->height,flame->ren_win.ren,graphics->color);
            break;
        case GraphicsTypes::sprite: {
            SDL_Rect dest = { graphics->x_pos,graphics->y_pos, 100, 100 };
            SDL_RenderCopy(flame->ren_win.ren, graphics->sprite, NULL, &dest);
            break;
        }
        default:
            break;
    }
}
void GameObject::update() {
    for (std::function<void(GameObject*,Flame*)> element: functions)
        element(this,flame);
}