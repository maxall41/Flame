#include <stdlib.h>
#include <SDL2/SDL.h>
#include <tuple>
#include <functional>
#include <vector>
#include <iostream>

struct RenWindowHandler {
    SDL_Renderer *ren;
    SDL_Window *win;
};

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

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
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

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
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

void render_pass_start(RenWindowHandler ren_win) {
    SDL_RenderClear(ren_win.ren);
}

void Shutdown(RenWindowHandler ren_win) {
    SDL_DestroyRenderer(ren_win.ren);
    SDL_Quit();
}

void CommitFrame(RenWindowHandler ren_win) {
    SDL_RenderPresent(ren_win.ren);
}

enum GraphicsTypes { circle_outline, circle_filled, rectangle_filled, rectangle_outline, sprite };


struct GameObjectGraphics {
    GraphicsTypes type;
    int radius;
    SDL_Color color;
    int x_pos;
    int y_pos;
    int width;
    int height;
    SDL_Texture sprite;
};

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
            case GraphicsTypes::rectangle_filled:
                DrawFilledRectangle(graphics->x_pos,graphics->y_pos,graphics->width,graphics->height,flame->ren_win.ren,graphics->color);
            case GraphicsTypes::sprite:
                SDL_RenderCopy(flame->ren_win.ren, graphics->sprite, &textureRect, &windowRect);
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
    graphics->type = GraphicsTypes::rectangle_filled;
    gameObject->assign_graphics(graphics,flame);
    gameObject->render();
}

int main(void)
{
    Flame* flame = new Flame(1000,500,"Flame engine testing");

    SDL_Color color = {255, 255, 255};

    SDL_Surface * image = SDL_LoadBMP("image.bmp");
    SDL_Texture * texture = SDL_CreateTextureFromSurface(flame->ren_win.ren, image);

    GameObject* g = new GameObject();
    GameObjectGraphics* graphics = new GameObjectGraphics();
    graphics->width = 100;
    graphics->height = 100;
    graphics->y_pos = 0;
    graphics->x_pos = 0;
    graphics->type = GraphicsTypes::rectangle_filled;
    g->assign_graphics(graphics,flame);
    g->add_behavior(movement);

    //While application is running
    while(!flame->quit)
    {
        render_pass_start(flame->ren_win);
        g->update();
        CommitFrame(flame->ren_win);
        flame->cycle();
        SDL_Delay(10);
    }

    Shutdown(flame->ren_win);

    return (0);
}