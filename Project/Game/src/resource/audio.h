#ifndef PLATFORMER_RESOURCE_AUDIO_H
#define PLATFORMER_RESOURCE_AUDIO_H

#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Music.hh>
#include <string>

//! Interface for a audio provider which is provided to a service locator
class Audio {
   public:
    enum class ID { kBackground, kCoin };
    virtual ~Audio() {}

    //! Put Background Music into Map
    virtual void PutMusic(Audio::ID audio_id, const std::string& file_path) = 0;
    //! Put Sound Effects into Map
    virtual void PutSoundEffect(Audio::ID audio_id,
                                const std::string& file_path) = 0;
    //! Get Background Music by ID
    virtual SDL2pp::Music& GetMusic(Audio::ID audio_id) = 0;
    //! Get Sound Effects by ID
    virtual SDL2pp::Chunk& GetSoundEffect(Audio::ID audio_id) = 0;
};

#endif
