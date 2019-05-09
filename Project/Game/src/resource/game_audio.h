#ifndef PLATFORMER_RESOURCE_GAME_AUDIO_H
#define PLATFORMER_RESOURCE_GAME_AUDIO_H

#include <memory>
#include <unordered_map>
#include "resource/audio.h"

//! An implementation of Audio
/*!
 * Only one instance is allowed to be instantiated.
 */
class GameAudio : public Audio {
   public:
    GameAudio();
    //! Forbids copying
    GameAudio(const GameAudio &) = delete;
    //! Forbids copying
    GameAudio &operator=(const GameAudio &) = delete;

    //! Put Background Music into this provider in "Assets" directory
    void PutMusic(Audio::ID audio_id, const std::string &file_basename);
    //! Put Sound Effects into this provider in "Assets" directory
    void PutSoundEffect(Audio::ID audio_id, const std::string &file_basename);
    //! Get Background Music by ID
    SDL2pp::Music &GetMusic(Audio::ID audio_id);
    //! Get Sound Effects by ID
    SDL2pp::Chunk &GetSoundEffect(Audio::ID audio_id);

   private:
    const std::string asset_dir_ {"../Assets/"};
    bool instantiated_ {false};
    std::unordered_map<Audio::ID, std::unique_ptr<SDL2pp::Music>> music_map_;
    std::unordered_map<Audio::ID, std::unique_ptr<SDL2pp::Chunk>> chunk_map_;
};

#endif
