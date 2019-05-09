#include "resource/game_audio.h"
#include <iostream>

GameAudio::GameAudio() {
    assert(!instantiated_);
    instantiated_ = true;
}

void GameAudio::PutMusic(Audio::ID audio_id, const std::string &file_basename) {
    music_map_[audio_id] =
        std::make_unique<SDL2pp::Music>(asset_dir_ + file_basename);
}

SDL2pp::Music &GameAudio::GetMusic(Audio::ID audio_id) {
    return *music_map_[audio_id];
}

void GameAudio::PutSoundEffect(Audio::ID audio_id,
                               const std::string &file_basename) {
    chunk_map_[audio_id] =
        std::make_unique<SDL2pp::Chunk>(asset_dir_ + file_basename);
}

SDL2pp::Chunk &GameAudio::GetSoundEffect(Audio::ID audio_id) {
    return *chunk_map_[audio_id];
}
