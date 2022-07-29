#include <SDL_ttf.h>
#include "SDL.h"
#include "core.h"
#include "graphics.h"
#include "fonts.h"
#include "sound.h"
#include <SDL_ttf.h>
#include <iostream>

int player_pos_x = 0;
int player_pos_y = 0;
int moveSpeed = 500;
SDL_Color clear_color = {0, 0, 0};
SDL_Texture* texture;
GameObject* g2;

void movement(GameObject* gameObject,Flame* flame) {
    if(flame->is_key_pressed(SDL_SCANCODE_LEFT)) {
        player_pos_x = player_pos_x - moveSpeed * flame->deltaTime;
    }
    if (flame->is_key_pressed(SDL_SCANCODE_RIGHT)) {
        player_pos_x = player_pos_x + moveSpeed * flame->deltaTime;
    }
    if(flame->is_key_pressed(SDL_SCANCODE_DOWN)) {
        player_pos_y = player_pos_y + moveSpeed * flame->deltaTime;
    }
    if (flame->is_key_pressed(SDL_SCANCODE_UP)) {
        player_pos_y = player_pos_y - moveSpeed * flame->deltaTime;
    }

    GameObjectGraphics* graphics = gameObject->graphics;
    graphics->x_pos = player_pos_x;
    graphics->y_pos = player_pos_y;
    gameObject->assign_graphics(graphics,flame);
}

void collison(GameObject* gameObject,Flame* flame) {
   std::cout << gameObject->is_colliding(g2);
}

int main(void)
{
    Flame* flame = new Flame(1000,500,"Flame engine testing");

    SDL_Color color = {255, 255, 255};
    SDL_Color color2 = {0, 0, 255};

    TTF_Font* font = LoadFont("font.ttf",80);

    SDL_Surface * image = Load_Image("block.bmp");
    texture = SDL_CreateTextureFromSurface(flame->ren_win.ren, image);
    SDL_FreeSurface(image);

    GameObject* g = new GameObject();
    GameObjectGraphics* graphics = new GameObjectGraphics();
    graphics->radius = 50;
    graphics->y_pos = 0;
    graphics->x_pos = 0;
    graphics->type = GraphicsTypes::circle_filled;
    graphics->color = color;
    g->assign_graphics(graphics,flame);
    g->add_behavior(movement);
    g->add_behavior(collison);


     g2 = new GameObject();
    GameObjectGraphics* graphics2 = new GameObjectGraphics();
    graphics2->y_pos = 100;
    graphics2->x_pos = 100;
    graphics2->type = GraphicsTypes::rectangle_filled;
    graphics2->color = color2;
    graphics2->width = 100;
    graphics2->height = 100;
    g2->assign_graphics(graphics2,flame);

    LoadedAudio testSound = load_wav_file("sound.wav");
    play_sound(testSound);
    //While application is running
    while(!flame->quit)
    {
        render_pass_start(flame->ren_win,clear_color);
        g->update();
        g->render();
        g2->render();
        DrawText(font,"Testing text",color,flame,0,0);
        flame->cycle(); // Run this just before committing the frame
        CommitFrame(flame->ren_win);
        SDL_Delay(17); // About 60 FPS
    }

    Shutdown(flame);

    return (0);
}
