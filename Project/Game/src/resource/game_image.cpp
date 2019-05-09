#include "resource/game_image.h"
#include <SDL.h>

GameImage::GameImage() {
    assert(!instantiated_);
    instantiated_ = true;
}

void GameImage::PutImage(const ImageID &image_id, const std::string &filename,
                         int x, int y, int w, int h) {
    if (image_id == kNullImageID) {
        return;
    }
    // Check if we loaded this image file before
    auto iter_texture {file_texture_map_.find(filename)};
    if (iter_texture == file_texture_map_.end()) {
        auto new_texture = std::make_shared<SDL2pp::Texture>(
            *renderer_, asset_dir_ + filename);
        image_textures_[image_id] =
            std::make_pair(std::shared_ptr<SDL2pp::Texture>(new_texture),
                           Math::Rect<int>(x, y, w, h));
        file_texture_map_[filename] = new_texture;
    } else {
        image_textures_[image_id] = std::make_pair(
            std::shared_ptr<SDL2pp::Texture>(iter_texture->second),
            Math::Rect<int>(x, y, w, h));
    }
    printf("[GameImage][PutImage] id: %s, file: %s, dimen: %d,%d,%d,%d\n",
           image_id.c_str(), filename.c_str(), x, y, w, h);
}

void GameImage::PutImage(const ImageID &image_id, const std::string &filename,
                         std::vector<int> ints) {
    assert(ints.size() == 4);
    PutImage(image_id, filename, ints[0], ints[1], ints[2], ints[3]);
}

void GameImage::RenderImage(const ImageID &id, const Math::Vec2<double> &dest,
                            bool flip_horz) {
    if (id == kNullImageID) {
        return;
    }

    auto image = image_textures_[id];
    renderer_->Copy(
        *image.first, image.second.toSDLRect(),
        SDL2pp::Rect((int)dest.x, (int)dest.y, image.second.w, image.second.h),
        0, SDL2pp::NullOpt, flip_horz ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void GameImage::PutSprite(Image::SpriteID sprite_id,
                          const std::string &filename,
                          std::initializer_list<Math::Rect<int>> rects) {
    std::vector<Math::Rect<int>> vec(rects.begin(), rects.end());
    PutSprite(sprite_id, filename, vec.begin(), vec.end());
}

void GameImage::PutSprite(Image::SpriteID sprite_id,
                          const std::string &filename,
                          std::vector<Math::Rect<int>>::iterator first,
                          std::vector<Math::Rect<int>>::iterator last) {
    if (sprite_id == kNullSpriteID) {
        return;
    }

    auto iter_texture {file_texture_map_.find(filename)};
    if (iter_texture == file_texture_map_.end()) {
        auto new_texture = std::make_shared<SDL2pp::Texture>(
            *renderer_, asset_dir_ + filename);
        sprites_[sprite_id] =
            std::make_pair(std::shared_ptr<SDL2pp::Texture>(new_texture),
                           std::vector<Math::Rect<int>>(first, last));
        file_texture_map_[filename] = new_texture;
    } else {
        sprites_[sprite_id] = std::make_pair(
            std::shared_ptr<SDL2pp::Texture>(iter_texture->second),
            std::vector<Math::Rect<int>>(first, last));
    }
    printf("[GameImage][PutSprite] id: %s, file: %s\n", sprite_id.c_str(),
           filename.c_str());
}

void GameImage::PutSprite(Image::SpriteID sprite_id,
                          const std::string &filename,
                          std::vector<std::vector<int>> ints) {
    assert(ints.size() > 0);
    assert(ints[0].size() == 4);
    std::vector<Math::Rect<int>> rects;
    for (auto rect_ints : ints) {
        rects.push_back(Math::Rect<int>(rect_ints));
    }
    PutSprite(sprite_id, filename, rects.begin(), rects.end());
}

void GameImage::RenderSprite(Image::SpriteID sprite_id, int index,
                             const Math::Vec2<double> &dest, bool flip_horz) {
    if (sprite_id == kNullSpriteID) {
        return;
    }

    auto &sprite = sprites_[sprite_id];
    assert(index >= 0);
    assert(index < sprite.second.size());
    auto &sprite_rect = sprite.second[index];
    renderer_->Copy(
        *sprite.first, sprite_rect.toSDLRect(),
        SDL2pp::Rect((int)dest.x, (int)dest.y, sprite_rect.w, sprite_rect.h), 0,
        SDL2pp::NullOpt, flip_horz ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}
