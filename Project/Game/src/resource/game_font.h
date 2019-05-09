#ifndef PLATFORMER_RESOURCE_GAME_FONT_H
#define PLATFORMER_RESOURCE_GAME_FONT_H

#include <memory>
#include <unordered_map>
#include "resource/font.h"

//! An implementation of Font
/*!
 * Only one instance is allowed to be instantiated.
 */
class GameFont : public Font {
   public:
    GameFont();
    //! Forbids copying
    GameFont(const GameFont &) = delete;
    //! Forbids copying
    GameFont &operator=(const GameFont &) = delete;

    //! Put Font into this provider in "Assets" directory
    void PutFont(Font::ID font_id, const std::string &file_basename, int size);
    //! Get Font by ID
    SDL2pp::Font &GetFont(Font::ID font_id);

   private:
    const std::string asset_dir_ {"../Assets/"};
    bool instantiated_ {false};
    std::unordered_map<Font::ID, std::unique_ptr<SDL2pp::Font>> font_map_;
};

#endif
