//
// Created by Max Campbell on 7/28/22.
//

#ifndef FLAME_ENGINE_SOUND_H
#define FLAME_ENGINE_SOUND_H

#include <SDL_mixer.h>
#include "core.h"

void SOUND_INIT();
struct LoadedAudio {
    Mix_Chunk* audio;
};
LoadedAudio load_wav_file(char* file_path);
void play_sound(LoadedAudio audio);
#endif //FLAME_ENGINE_SOUND_H
