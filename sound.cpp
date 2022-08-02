#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include "include/sound.h"

LoadedAudio load_wav_file(char* path) {
    Mix_Chunk* audio = Mix_LoadWAV(path);
    return {audio};
}

void play_sound(LoadedAudio loadedAudio) {
    Mix_PlayChannel(-1, loadedAudio.audio, 0);
}

void play_sound_looped(LoadedAudio loadedAudio) {
    Mix_PlayChannel(-1, loadedAudio.audio, -1);
}

void SOUND_INIT() {
    int result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    if(result < 0) {
        std::cout << "Failed to init audio " << Mix_GetError();
    }
    Mix_AllocateChannels(32); // This should be fine for most games
}