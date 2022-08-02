#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <functional>
#include <iostream>
#include "include/graphics.h"
#include "include/core.h"
#include "include/sound.h"

void Shutdown(Flame* flame) {
    SDL_DestroyRenderer(flame->ren_win.ren);
    SDL_Quit();
}

Flame::Flame(int window_x_size, int window_y_size,char* title) {
    // Init various libs
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(0);
    SOUND_INIT();
    // Create window
    SDL_Window* win = SDL_CreateWindow(title,0,0,window_x_size,window_y_size,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    // Create Renderer
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    // Return Renderer Window struct
    ren_win = {ren,win};
}

bool Flame::is_key_pressed(Uint8 key) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    return state[key];
}

bool Flame::is_left_mouse_click() {
    const Uint32 state = SDL_GetMouseState(NULL,NULL);
    return (state & SDL_BUTTON_LMASK) != 0;
}

bool Flame::is_right_mouse_click() {
    const Uint32 state = SDL_GetMouseState(NULL,NULL);
    return (state & SDL_BUTTON_RMASK) != 0;
}

bool Flame::is_middle_mouse_click() {
    const Uint32 state = SDL_GetMouseState(NULL,NULL);
    return (state & SDL_BUTTON_MMASK) != 0;
}

void Flame::cycle(void) {
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();

    Flame::deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() ) * 0.001;
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
    // Draw the text that the user has created
    Flame::DrawText();
}

void Flame::DrawText() {
    for(TextData* textData : Flame::draw_text) {
        SDL_Rect sizing; //create a rect
        sizing.x = textData->pos_x;  //controls the rect's x coordinate
        sizing.y = textData->pos_y; // controls the rect's y coordinte
        sizing.w = textData->width; // controls the width of the rect
        sizing.h = textData->height; // controls the height of the rect
        SDL_RenderCopy(Flame::ren_win.ren, textData->texture, NULL, &sizing);
        SDL_DestroyTexture(textData->texture);
    }
    Flame::draw_text.clear();
}

void GameObject::assign_graphics(GameObjectGraphics* set_graphics,Flame* set_flame) {
    graphics = set_graphics;
    flame = set_flame;
}


void GameObject::add_behavior(std::function<void(GameObject*,Flame*)> behavior) {
    functions.push_back(behavior);
}

bool GameObject::is_rect_colliding(GameObject* gameObject) {
   SDL_Rect a = { gameObject->graphics->x_pos,gameObject->graphics->y_pos, gameObject->graphics->width, gameObject->graphics->height };
   SDL_Rect b = { GameObject::graphics->x_pos,GameObject::graphics->y_pos, GameObject::graphics->width, GameObject::graphics->height };
   SDL_bool intersecting = SDL_HasIntersection(&a,&b);
   return intersecting == SDL_bool::SDL_TRUE;
}

double distanceSquared( int x1, int y1, int x2, int y2 )
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX*deltaX + deltaY*deltaY;
}

bool GameObject::is_circle_and_rect_colliding(GameObject* gameObject) {
    GameObject* circle;
    GameObject* rect;
    if (gameObject->graphics->type == GraphicsTypes::circle_outline || gameObject->graphics->type == GraphicsTypes::circle_filled) {
        circle = gameObject;
        rect = this;
    } else {
        circle = this;
        rect = gameObject;
    }
    //Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if( circle->graphics->x_pos < rect->graphics->x_pos )
    {
        cX = rect->graphics->x_pos;
    }
    else if( circle->graphics->x_pos > rect->graphics->x_pos + rect->graphics->width )
    {
        cX = rect->graphics->x_pos + rect->graphics->width;
    }
    else
    {
        cX = circle->graphics->x_pos;
    }

    //Find closest y offset
    if( circle->graphics->y_pos  < rect->graphics->y_pos )
    {
        cY = rect->graphics->y_pos;
    }
    else if( circle->graphics->y_pos  > rect->graphics->y_pos + rect->graphics->height )
    {
        cY = rect->graphics->y_pos + rect->graphics->height;
    }
    else
    {
        cY = circle->graphics->y_pos;
    }

    //If the closest point is inside the circle
    if( distanceSquared( circle->graphics->x_pos, circle->graphics->y_pos, cX, cY ) < circle->graphics->radius * circle->graphics->radius )
    {
        //This box and the circle have collided
        return true;
    }


    //If the shapes have not collided
    return false;
}

bool GameObject::is_circle_colliding(GameObject *gameObject) {
    //Calculate total radius squared
    int totalRadiusSquared = GameObject::graphics->radius + gameObject->graphics->radius;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distanceSquared( GameObject::graphics->x_pos, GameObject::graphics->y_pos, gameObject->graphics->x_pos, gameObject->graphics->y_pos ) < ( totalRadiusSquared ) )
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

bool GameObject::is_colliding(GameObject* gameObject) {
 if ((GameObject::graphics->type == GraphicsTypes::rectangle_filled || GameObject::graphics->type == GraphicsTypes::rectangle_outline) && (gameObject->graphics->type == GraphicsTypes::rectangle_filled || gameObject->graphics->type == GraphicsTypes::rectangle_outline)) {
     return is_rect_colliding(gameObject);
 }
    else if ((GameObject::graphics->type == GraphicsTypes::circle_filled || GameObject::graphics->type == GraphicsTypes::circle_outline) && (gameObject->graphics->type == GraphicsTypes::circle_filled || gameObject->graphics->type == GraphicsTypes::circle_outline)) {
        return is_circle_colliding(gameObject);
    }
 else if ((GameObject::graphics->type == GraphicsTypes::circle_filled || GameObject::graphics->type == GraphicsTypes::circle_outline) && (gameObject->graphics->type == GraphicsTypes::rectangle_filled || gameObject->graphics->type == GraphicsTypes::rectangle_outline)) {
     return is_circle_and_rect_colliding(gameObject);
 }
    else {
        std::cout << "Collisions not supported for these types";
    }
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