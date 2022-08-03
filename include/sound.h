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

class MusicManager {
private:
    std::vector<Mix_Music*> tracks;
    int fade_time;
    double end_time{};
    static bool next_flag;
    static void music_finished();
public:
    bool auto_next;
    bool fade;
    int current_track{};
    MusicManager(bool new_fade,bool new_auto_next,const std::vector<char*>& new_tracks,int new_fade_time);
    void next();
    void back();
    void resume();
    void pause();
    void play();
    void add_tracks(std::vector<char *> new_tracks);
    void cycle();
};

LoadedAudio load_wav_file(char* file_path);

void play_sound(LoadedAudio audio);
#endif //FLAME_ENGINE_SOUND_H
