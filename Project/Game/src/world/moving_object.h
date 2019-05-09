#ifndef PLATFORMER_WORLD_MOVING_OBJECT_H
#define PLATFORMER_WORLD_MOVING_OBJECT_H

#include "object/object.h"
#include "world/camera.h"

//! An object that can move in the tile map
class MovingObject {
   public:
    //! Constructor
    MovingObject(std::shared_ptr<Object> object, const CameraSystem& camera)
        : object_(object) {
        old_relpos_win_ = object->GetPos();
        relpos_map_ = camera.ConvertFromMapRelToViewportRelPos(old_relpos_win_);
    }

   private:
    std::shared_ptr<Object> object_;
    Math::Vec2<double> old_relpos_win_;
    Math::Vec2<double> relpos_map_;
};

#endif
