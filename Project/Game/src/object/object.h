#ifndef PLATFORMER_OBJECT_OBJECT_H
#define PLATFORMER_OBJECT_OBJECT_H

#include <SDL.h>
#include <SDL2pp/Rect.hh>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Texture.hh>
#include <utility>
#include "resource/base.h"
#include "tinymath.h"

//! %Base class for objects that can be rendered at certain position
/*!
 * Specifically, it has a position, velocity (maybe 0), etc.
 * Every object has a texture object which has the information of how to
 * render it on the screen (i.e. position of the texture on its source
 * image, SDL2 texture object etc.).
 * A CollidableObject has a collision object which defines the boundary
 * for collision detection algorithm.
 * Note that the position and dimension of a texture object and a
 * collision object might be different from each other. Thus the two
 * objects need to be initialized separately.
 */
class Object : public Base {
   public:
    enum class ImageType { kStatic, kSprite };

    Object() {}
    Object(ImageType type, const std::string& image_id)
        : image_type_(type), image_id_(image_id) {}
    virtual ~Object() {}

    //! Getter for x position in SDL coordinate system
    double pos_x() const { return pos_.x; }
    //! Getter for y position in SDL coordinate system
    double pos_y() const { return pos_.y; }
    //! Returns position in SDL coordinate system as a Vec2
    Math::Vec2<double> GetPos() const { return pos_; }

    //! Sets x and y position
    virtual void SetPos(double pos_x, double pos_y) { SetPos({pos_x, pos_y}); }
    //! Set the position to a 2D vector
    virtual void SetPos(Math::Vec2<double> pos) { pos_ = pos; }
    //! Adds an offset vector onto current position
    virtual void AdjustPos(const Math::Vec2<double>& offset) {
        pos_.Add(offset);
    }

    //! Getter for x velocity
    double vel_x() const { return vel_.x; }
    //! Getter for y velocity
    double vel_y() const { return vel_.y; }
    //! Setter for x velocity
    void set_velx(double vel_x) { vel_.x = vel_x; }
    //! Setter for y velocity
    void set_vely(double vel_y) { vel_.y = vel_y; }
    //! Sets x and y velocity
    void SetVel(double vel_x, double vel_y) {
        vel_.x = vel_x;
        vel_.y = vel_y;
    }
    //! Set the velocity
    void SetVel(Math::Vec2<double> vel) { vel_ = vel; }
    //! Returns velociy as a Vec2
    Math::Vec2<double> GetVel() const { return vel_; }

    //! Returns dimension of its texture
    virtual std::pair<int, int> GetTextureDimen() const {
        if (image_type_ == ImageType::kStatic)
            return GetImageProvider().GetImageDimen(image_id());
        else
            return GetImageProvider().GetSpriteDimen(image_id());
    }

    //! Updates position according to velocity
    virtual void Update(int ms_passed) {
        pos_.Add(Math::Scale(vel_, ms_passed / 1000.0));
    }
    //! Renders object at its position
    virtual void Render(int ms_passed) { Render(pos_, ms_passed); }
    //! Renders object at an arbitrary position
    virtual void Render(double x, double y, int ms_passed) {
        Render({x, y}, ms_passed);
    }
    //! Render to the screen
    virtual void Render(Math::Vec2<double> pos, int ms_passed);

    //! Process the input
    virtual void ProcessInput(SDL_Event e) {}

    //! Returns name of this object
    virtual std::string GetName() const { return "object"; }
    //! Returns a string representation
    virtual std::string to_string() const;

    //! Gets image id
    virtual Image::ImageID image_id() const { return image_id_; }

   protected:
    Math::Vec2<double> pos_ {0, 0};
    Math::Vec2<double> vel_ {0, 0};

   private:
    //! Flag for image or sprite
    ImageType image_type_ {ImageType::kStatic};
    Image::ImageID image_id_ {Image::kNullImageID};
};

#endif
