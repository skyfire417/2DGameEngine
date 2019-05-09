#ifndef PLATFORMER_OBJECT_FALLABLE_OBJECT_H
#define PLATFORMER_OBJECT_FALLABLE_OBJECT_H

#include <json/json.h>
#include "object/collidable_object.h"

//! %Base class for fallable objects
/*!
 * A fallable object is an object that will be affected by the gravity.
 */
class FallableObject : public CollidableObject {
   public:
    FallableObject(CollidableObject::Type type) : CollidableObject(type) {}
    FallableObject(CollidableObject::Type type, ImageType image_type,
                   Image::ImageID image_id)
        : CollidableObject(type, image_type, image_id) {}

    //! Read gravity setting from a JSON config
    static void ReadConfiguration();

    //! Returns vertical velocity change per second
    double gravity() { return gravity_; }

    //! Update the location of the object
    void Update(int ms_passed) {
        set_vely(vel_y() + gravity_ * ms_passed / 1000);
        CollidableObject::Update(ms_passed);
    }

   private:
    static double gravity_;
};

#endif
