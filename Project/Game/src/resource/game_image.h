#ifndef PLATFORMER_RESOURCE_GAME_IMAGE_H
#define PLATFORMER_RESOURCE_GAME_IMAGE_H

#include <string>
#include <unordered_map>
#include "resource/image.h"

//! An implementation of Image
/*!
 * Only one instance is allowed to be instantiated.
 */
class GameImage : public Image {
   public:
    GameImage();
    //! Forbids copying
    GameImage(const GameImage &) = delete;
    //! Forbids copying
    GameImage &operator=(const GameImage &) = delete;

    //! Get dimension of Image
    std::pair<int, int> GetImageDimen(const ImageID &image_id) {
        if (image_id == kNullImageID) {
            return {0, 0};
        }
        auto &rect = image_textures_[image_id].second;
        return {rect.w, rect.h};
    }

    //! Put Image into this provider in "Assets" directory
    void PutImage(const ImageID &image_id, const std::string &filename, int x,
                  int y, int w, int h);
    void PutImage(const ImageID &image_id, const std::string &filename,
                  std::vector<int> ints);

    //! Render Image to the screen
    void RenderImage(const ImageID &id, const Math::Vec2<double> &dest,
                     bool flip_horz = false);

    //! Put a spritesheet into this provider in "Assets" directory
    void PutSprite(Image::SpriteID sprite_id, const std::string &filename,
                   std::initializer_list<Math::Rect<int>> rects);
    void PutSprite(Image::SpriteID sprite_id, const std::string &filename,
                   std::vector<Math::Rect<int>>::iterator first,
                   std::vector<Math::Rect<int>>::iterator last);
    void PutSprite(Image::SpriteID sprite_id, const std::string &filename,
                   std::vector<std::vector<int>> ints);

    //! Get frame count of a sprite
    int GetFrameCount(Image::SpriteID sprite_id) {
        return sprites_[sprite_id].second.size();
    }

    //! Returns dimension of a sprite image
    //! Assumes every sprite of one kind has same dimension.
    std::pair<int, int> GetSpriteDimen(Image::SpriteID sprite_id) {
        auto &rect = sprites_[sprite_id].second[0];
        return {rect.w, rect.h};
    }
    //! Render sprite to the screen
    void RenderSprite(Image::SpriteID sprite_id, int index,
                      const Math::Vec2<double> &dest, bool flip_horz = false);

   private:
    const std::string asset_dir_ {"../Assets/"};

    bool instantiated_ {false};
    std::unordered_map<
        ImageID, std::pair<std::shared_ptr<SDL2pp::Texture>, Math::Rect<int>>>
        image_textures_;
    std::unordered_map<std::string, std::shared_ptr<SDL2pp::Texture>>
        file_texture_map_;
    //! Sprites resource
    /*!
     * Sprites of one type all sit in the same texture (image).
     */
    std::unordered_map<Image::SpriteID,
                       std::pair<std::shared_ptr<SDL2pp::Texture>,
                                 std::vector<Math::Rect<int>>>>
        sprites_;
};

#endif
