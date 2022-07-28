#include <SDL_ttf.h>
#include "SDL.h"
#include "core.h"
#include "graphics.h"
#include "fonts.h"
#include <SDL_ttf.h>

int player_pos_x = 0;
int player_pos_y = 0;
int moveSpeed = 500;
SDL_Color clear_color = {0, 0, 0};
SDL_Texture* texture;

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

    SDL_Color color = {255, 255, 255};
    GameObjectGraphics* graphics = gameObject->graphics;
    graphics->x_pos = player_pos_x;
    graphics->y_pos = player_pos_y;
    gameObject->assign_graphics(graphics,flame);
    gameObject->render();
}

int main(void)
{
    Flame* flame = new Flame(1000,500,"Flame engine testing");

    SDL_Color color = {255, 255, 255};

    TTF_Font* font = LoadFont("font.ttf",80);

    SDL_Surface * image = Load_Image("block.bmp");
    texture = SDL_CreateTextureFromSurface(flame->ren_win.ren, image);
    SDL_FreeSurface(image);

    GameObject* g = new GameObject();
    GameObjectGraphics* graphics = new GameObjectGraphics();
    graphics->width = 100;
    graphics->height = 100;
    graphics->y_pos = 0;
    graphics->x_pos = 0;
    graphics->sprite = texture;
    graphics->type = GraphicsTypes::sprite;
    g->assign_graphics(graphics,flame);
    g->add_behavior(movement);


    //While application is running
    while(!flame->quit)
    {
        render_pass_start(flame->ren_win,clear_color);
        g->update();
        DrawText(font,"Testing text",color,flame,0,0);
        flame->cycle(); // Run this just before commiting the frame
        CommitFrame(flame->ren_win);
        SDL_Delay(10);
    }

    Shutdown(flame->ren_win);

    return (0);
}
