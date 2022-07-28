//
// Created by Max Campbell on 7/27/22.
//

#ifndef FLAME_ENGINE_GRAPHICS_H
#define FLAME_ENGINE_GRAPHICS_H

#include <SDL.h>


enum GraphicsTypes { circle_outline, circle_filled, rectangle_filled, rectangle_outline, sprite };


struct GameObjectGraphics {
    GraphicsTypes type;
    int radius;
    SDL_Color color;
    int x_pos;
    int y_pos;
    int width;
    int height;
    SDL_Texture* sprite;
};

struct RenWindowHandler {
    SDL_Renderer *ren;
    SDL_Window *win;
};

void DrawCircleOutline(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius, SDL_Color color);
void DrawOutlineRectangle(int x_pos,int y_pos,int width,int height,SDL_Renderer* ren,SDL_Color color);
void DrawFilledRectangle(int x_pos,int y_pos,int width,int height,SDL_Renderer* ren,SDL_Color color);
void DrawFilledCircle(SDL_Renderer *renderer, int x_pos, int y_pos, int radius, SDL_Color color);
void render_pass_start(RenWindowHandler ren_win,SDL_Color color);
void CommitFrame(RenWindowHandler ren_win);

#endif //FLAME_ENGINE_GRAPHICS_H
