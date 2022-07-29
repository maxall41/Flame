#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "core.h"
#include "fonts.h"

TTF_Font* LoadFont(char* file_path,int font_size) {
    //this opens a font style and sets a size
    TTF_Font *font = TTF_OpenFont(file_path, font_size);
    assert(font);
    return font;
}

void DrawText(TTF_Font* font,char* text,SDL_Color color,Flame* flame,int pos_x,int pos_y) {
    SDL_Surface* surfaceMessage =
            TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(flame->ren_win.ren, surfaceMessage);
    TextData* textData = new TextData();
    textData->pos_y = pos_y;
    textData->pos_x = pos_x;
    textData->texture = texture;
    textData->width = surfaceMessage->w;
    textData->height = surfaceMessage->h;
    SDL_FreeSurface(surfaceMessage);
    flame->draw_text.push_back(textData);
}