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

bool MusicManager::next_flag = false;

void MusicManager::music_finished() {
    next_flag = true;
}

MusicManager::MusicManager(bool new_fade, bool new_auto_next, const std::vector<char *>& new_tracks,int new_fade_time) {
    std::vector<Mix_Music *> load_tracks;
    // Preload tracks
    for(char* track: new_tracks)
        load_tracks.push_back(Mix_LoadMUS(track));
    // Set vars
    MusicManager::tracks = load_tracks;
    MusicManager::fade = new_fade;
    MusicManager::fade_time = new_fade_time;
    MusicManager::auto_next = new_auto_next;
    Mix_HookMusicFinished(this->music_finished);
}

void MusicManager::cycle() {
    if (next_flag) {
        next_flag = false;
        this->next();
    }
}

void MusicManager::play() {
    Mix_Music* current_track_chunk = tracks[current_track];
    if (fade) {
        Mix_FadeInMusic(current_track_chunk,0,fade_time);
    } else {
        Mix_PlayMusic(current_track_chunk,0);
    }
    // Update current track
    current_track++;
}

void MusicManager::next() {
    if (fade) {
        Mix_FadeOutMusic(fade_time);
        Mix_FadeInMusic(tracks[current_track],0,fade_time);
        current_track++;
    }
}

void MusicManager::add_tracks(std::vector<char *> new_tracks) {
    std::vector<Mix_Music *> load_tracks;
    // Preload tracks
    for(char* track: new_tracks)
        load_tracks.push_back(Mix_LoadMUS(track));
    // Save
    MusicManager::tracks = load_tracks;
}

void MusicManager::back() {
    if (fade) {
        Mix_FadeOutMusic(fade_time);
        current_track -= 2;
        Mix_FadeInMusic(tracks[current_track],0,fade_time);
    }
}


void MusicManager::pause() {
    if (fade) {
        Mix_FadeOutMusic(fade_time);
    } else {
        Mix_PauseMusic();
    }
}

void MusicManager::resume() {
    Mix_ResumeMusic();
}

void SOUND_INIT() {
    int result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    if(result < 0) {
        std::cout << "Failed to init audio " << Mix_GetError();
    }
    Mix_AllocateChannels(32); // This should be fine for most games
}