#ifndef PLATFORMER_OBJECT_BORDER_H
#define PLATFORMER_OBJECT_BORDER_H

#include <memory>
#include "object/collidable_object.h"

//! %Base class for borders
/*!
 * A collidable border is an object that the charactor and enemies can not go through.
 * It can be specified by giving starting point and length.
 */
class Border : public CollidableObject {
   public:
    enum class Side { kTop, kRight, kBottom, kLeft };

    Border(Side side)
        : side_(side), CollidableObject(CollidableObject::Type::kBorder) {}

    /*!
     * Set position of a border object by specifying a starting point and
     * length. The direction of where the border extends from the starting point
     * follows the direction of axes, e.g. (0, 0) -> (0, 100),
     * (0, 0) -> (100, 0)
     */
    void SetPos(Math::Vec2<int> start_point, int length);

   private:
    static const int kDepth {50};

    Side side_;
};

#endif
