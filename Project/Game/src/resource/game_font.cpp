#include "resource/game_font.h"

GameFont::GameFont() {
    assert(!instantiated_);
    instantiated_ = true;
}

void GameFont::PutFont(Font::ID font_id, const std::string &file_basename,
                       int size) {
    font_map_[font_id] =
        std::make_unique<SDL2pp::Font>(asset_dir_ + file_basename, size);
}

SDL2pp::Font &GameFont::GetFont(Font::ID font_id) {
    return *font_map_[font_id];
}
