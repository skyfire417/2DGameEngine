#ifndef PLATFORMER_RESOURCE_IMAGE_H
#define PLATFORMER_RESOURCE_IMAGE_H

#include <SDL2pp/Rect.hh>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Texture.hh>
#include <functional>
#include <memory>
#include <utility>
#include "tinymath.h"

//! Interface for an image provider which is provided to a service locator
class Image {
   public:
    //! String that uniquely identifies an image file or part of one
    /*!
     * Often we clip an image to reuse one to render multiple
     * images.
     */
    typedef std::string ImageID;
    static const std::string kNullImageID;

    typedef std::string SpriteID;
    static const std::string kNullSpriteID;

    virtual ~Image() {}

    //! Provides a renderer to this image provider.
    /*!
     * A renderer should be provided during initialization. Use of
     * `unique_ptr` hints that the ownership of the renderer will be
     * transferred to locator and whoever initializes the renderer
     * should not keep reference/raw pointer to it (e.g. assign it
     * to a member of object)
     */
    void ProvideRenderer(std::unique_ptr<SDL2pp::Renderer> renderer) {
        renderer_ = std::move(renderer);
    }

    //! Gets a renderer from this image provider.
    /*!
     * Returning a reference implies that caller should not keep the
     * reference to prolong the lifetime of this object (e.g. assign
     * it to a member of object)
     */
    SDL2pp::Renderer& GetRenderer() { return *renderer_; }

    //! Puts an image into this provider to be managed by it
    /*!
     * An image is specified by its ID, path to the image files,
     * top-left position on the image, and its dimension on the image.
     */
    virtual void PutImage(const ImageID& image_id, const std::string& file_path,
                          int x, int y, int w, int h) = 0;
    virtual void PutImage(const ImageID& image_id, const ImageID& filename,
                          std::vector<int> ints) = 0;

    //! Returns dimension of an image
    virtual std::pair<int, int> GetImageDimen(const ImageID& image_id) = 0;

    //! Put a spritesheet into this provider in "Assets" directory
    virtual void PutSprite(Image::SpriteID sprite_id,
                           const std::string& file_path,
                           std::initializer_list<Math::Rect<int>> rects) = 0;
    //! Put a spritesheet into this provider in "Assets" directory
    virtual void PutSprite(Image::SpriteID sprite_id,
                           const std::string& file_path,
                           std::vector<Math::Rect<int>>::iterator first,
                           std::vector<Math::Rect<int>>::iterator last) = 0;
    //! Put a spritesheet into this provider in "Assets" directory
    virtual void PutSprite(Image::SpriteID sprite_id,
                           const std::string& filename,
                           std::vector<std::vector<int>> ints) = 0;

    //! Get frame count of a sprite
    virtual int GetFrameCount(Image::SpriteID sprite_id) = 0;

    //! Returns dimension of a sprite image
    //! Assumes every sprite of one kind has same dimension.
    virtual std::pair<int, int> GetSpriteDimen(Image::SpriteID sprite_id) = 0;

    //! Render Image to the screen
    virtual void RenderImage(const ImageID& id, const Math::Vec2<double>& dest,
                             bool flip_horz = false) = 0;

    //! Render sprite to the screen
    virtual void RenderSprite(Image::SpriteID sprite_id, int index,
                              const Math::Vec2<double>& dest,
                              bool flip_horz = false) = 0;

   protected:
    std::unique_ptr<SDL2pp::Renderer> renderer_;
};

#endif
