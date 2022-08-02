#include <SDL.h>
#include "include/graphics.h"
#include <SDL_image.h>
#include <stdlib.h>
#include <assert.h>

void DrawCircleOutline(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius,SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void DrawOutlineRectangle(int x_pos,int y_pos,int width,int height,SDL_Renderer* ren,SDL_Color color) {

    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);

    SDL_Rect rect;
    rect.x = x_pos;
    rect.y = y_pos;
    rect.w = width;
    rect.h = height;

    SDL_RenderDrawRect(ren, &rect);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
}

void DrawFilledRectangle(int x_pos,int y_pos,int width,int height,SDL_Renderer* ren,SDL_Color color) {

    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);

    SDL_Rect rect;
    rect.x = x_pos;
    rect.y = y_pos;
    rect.w = width;
    rect.h = height;

    SDL_RenderFillRect(ren, &rect);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
}

void DrawFilledCircle(SDL_Renderer *renderer, int x_pos, int y_pos, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x_pos + dx, y_pos + dy);
            }
        }
    }
}

void render_pass_start(RenWindowHandler ren_win,SDL_Color color) {
    SDL_SetRenderDrawColor(ren_win.ren, color.r, color.g, color.b, color.a);
    SDL_RenderClear(ren_win.ren);
}

void CommitFrame(RenWindowHandler ren_win) {
    SDL_RenderPresent(ren_win.ren);
}

SDL_Surface* Load_Image(char* file_path) {
    SDL_Surface * image = IMG_Load(file_path);
    assert(image);
    return image;
}
