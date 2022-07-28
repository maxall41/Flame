#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <functional>
#include <vector>
#include <iostream>
#include "graphics.h"



void Shutdown(RenWindowHandler ren_win) {
    SDL_DestroyRenderer(ren_win.ren);
    SDL_Quit();
}

class Flame {
public:
    RenWindowHandler ren_win;
    SDL_Event event_handler;
    bool quit = false;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    Flame(int window_x_size, int window_y_size,char* title) {
        SDL_Init(SDL_INIT_EVERYTHING);
        IMG_Init(0);

        SDL_Window* win = SDL_CreateWindow(title,0,0,window_x_size,window_y_size,0);
        SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        SDL_RenderClear(ren);
        ren_win = {ren,win};
    }
    void cycle(void) {
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
    bool is_key_pressed(Uint8 key) {
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        return state[key];
    }
};

class GameObject {
public:
    GameObjectGraphics* graphics;
    Flame* flame;
    std::vector<std::function<void(GameObject*,Flame*)>> functions;

    GameObject ( )
    {
    }

    void assign_graphics(GameObjectGraphics* set_graphics,Flame* set_flame) {
        graphics = set_graphics;
        flame = set_flame;
    }

    void add_behavior(std::function<void(GameObject*,Flame*)> behavior) {
        functions.push_back(behavior);
    }

    void render() {
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
    void update() {
        for (std::function<void(GameObject*,Flame*)> element: functions)
            element(this,flame);
    }
};



// Everything below this line is the user using the flame engine

int player_pos_x = 0;
int player_pos_y = 0;
int moveSpeed = 500;
SDL_Color clear_color = {0, 0, 0};
SDL_Texture * texture;

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
    GameObjectGraphics* graphics = new GameObjectGraphics();
    graphics->width = 100;
    graphics->height = 100;
    graphics->x_pos = player_pos_x;
    graphics->y_pos = player_pos_y;
    graphics->sprite = texture;
    graphics->type = GraphicsTypes::sprite;
    gameObject->assign_graphics(graphics,flame);
    gameObject->render();
}

int main(void)
{
    Flame* flame = new Flame(1000,500,"Flame engine testing");

    SDL_Color color = {255, 255, 255};

    SDL_Surface * image = IMG_Load("block.bmp");
    assert(image);
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
        CommitFrame(flame->ren_win);
        flame->cycle();
        SDL_Delay(10);
    }

    Shutdown(flame->ren_win);

    return (0);
}