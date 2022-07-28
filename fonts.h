//
// Created by Max Campbell on 7/28/22.
//

#ifndef FLAME_ENGINE_FONTS_H
#include <SDL.h>
#include "core.h"


TTF_Font* LoadFont(char* file_path,int font_size);
void DrawText(TTF_Font* font,char* text,SDL_Color color,Flame* flame,int pos_x,int pos_y);
#define FLAME_ENGINE_FONTS_H

#endif //FLAME_ENGINE_FONTS_H
