#ifndef PLATFORMER_OBJECT_BORDER_MANAGER_H
#define PLATFORMER_OBJECT_BORDER_MANAGER_H

#include <functional>
#include <unordered_map>
#include "object/border.h"
#include "tinymath.h"

//! Manages Border objects
class BorderManager {
   public:
    BorderManager();
    //! Initializes borders to be managed by this manager
    /*!
     * It sets borders' on-collision functions to stop colliding objects.
     * It resets any other on-collision functions set previously.
     */
    void ResetPosition(const Math::Rect<int>& area);
    //! Returns all collision objects of Border objects that it manages
    std::vector<std::shared_ptr<CollidableObject>>
    //! Share border collidables 
    ShareBorderCollidableObjects();
    //! Sets on-collision function on one of the sides
    void SetOnCollisionFunc(
        Border::Side side,
        std::function<void(CollidableObject&, Math::Vec2<double>)> callback);

    Math::Rect<int> boundary() { return boundary_; }

   private:
    std::unordered_map<Border::Side, std::shared_ptr<Border>> borders_;
    Math::Rect<int> boundary_ {0, 0, 0, 0};
};

#endif
