#ifndef PLATFORMER_RESOURCE_NULL_IMAGE_H
#define PLATFORMER_RESOURCE_NULL_IMAGE_H

#include "resource/image.h"

//! Class for a fall back solution when something goes wrong.
class NullImage : public Image {
   public:
    //! Puts an image into this provider to be managed by it
    /*!
     * An image is specified by its ID, path to the image files,
     * top-left position on the image, and its dimension on the image.
     */
    void PutImage(const ImageID& image_id, const std::string& file_path, int x,
                  int y, int w, int h) {}
    void PutImage(const ImageID& image_id, const ImageID& filename,
                  std::vector<int> ints) {}

    //! Returns dimension of an image
    std::pair<int, int> GetImageDimen(const ImageID& image_id) {
        return {0, 0};
    }

    //! Render Image to the screen
    void RenderImage(const ImageID& id, const Math::Vec2<double>& dest,
                     bool flip_horz = false) {}

    //! Put a spritesheet into this provider in "Assets" directory
    void PutSprite(Image::SpriteID sprite_id, const std::string& file_path,
                   std::initializer_list<Math::Rect<int>> rects) {}
    //! Put a spritesheet into this provider in "Assets" directory
    void PutSprite(Image::SpriteID sprite_id, const std::string& file_path,
                   std::vector<Math::Rect<int>>::iterator first,
                   std::vector<Math::Rect<int>>::iterator last) {}
    //! Put a spritesheet into this provider in "Assets" directory
    void PutSprite(Image::SpriteID sprite_id, const std::string& filename,
                   std::vector<std::vector<int>> ints) {}

    //! Get frame count of a sprite
    int GetFrameCount(Image::SpriteID sprite_id) { return 0; }

    //! Returns dimension of a sprite image
    //! Assumes every sprite of one kind has same dimension.
    std::pair<int, int> GetSpriteDimen(Image::SpriteID sprite_id) {
        return {0, 0};
    }

    //! Render sprite to the screen
    void RenderSprite(Image::SpriteID sprite_id, int index,
                      const Math::Vec2<double>& dest, bool flip_horz = false) {}
};

#endif
