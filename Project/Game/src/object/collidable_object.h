#ifndef PLATFORMER_OBJECT_COLLIDABLE_OBJECT_H
#define PLATFORMER_OBJECT_COLLIDABLE_OBJECT_H

#include "object/object.h"

//! Object for collision detection
/*!
 * Every object that can be collided with another should inherit this
 * class and initialize the CollidableObject accordingly.
 * Because a collision object needs to hold a shared_ptr of its linked
 * Object, every subclass of this class has a factory method that handles
 * the initialization.
 */
class CollidableObject : public Object {
   public:
    enum class Type { kBorder, kObstacle, kPlayer, kEnemy, kCoin };

    CollidableObject(Type type) : type_(type) {}
    CollidableObject(Type type, ImageType image_type,
                     const std::string& image_id)
        : Object(image_type, image_id), type_(type) {}

    //! Setter for on-collision function
    void SetOnCollisionFunc(
        std::function<void(CollidableObject&, Math::Vec2<double>)> callback) {
        on_collisions_.clear();
        on_collisions_.push_back(callback);
    }

    //! Add function on collision
    void AddOnCollisionFunc(
        std::function<void(CollidableObject&, Math::Vec2<double>)> callback) {
        on_collisions_.push_back(callback);
    }

    //! Getter for type
    Type CollidObjType() const { return type_; }

    //! Returns dimension of this collision object
    std::pair<int, int> CollidObjDimen() const { return dimen_; }

    //! Sets dimension of this collision object
    void SetCollidObjDimen(int w, int h) {
        dimen_.first = w;
        dimen_.second = h;
    }

    //! Sets dimension of this collision object
    void SetCollidObjDimen(std::pair<int, int> dimen) { dimen_ = dimen; }

    /*! \brief Returns a rectangle which represents collision object's position
     *         and diemension.
     */
    Math::Rect<double> CollidObjRect() {
        return {pos_x() + collid_pos_offset_.x, pos_y() + collid_pos_offset_.y,
                (double)dimen_.first, (double)dimen_.second};
    }

    //! On-collision side effect function
    /*!
     * `displace_vec` is a vector that can move out this collision object
     * from the other, i.e. by adding `displace_vec` to position of this
     * object, the two objects no longer collide with each other.
     */
    void OnCollision(CollidableObject& other, Math::Vec2<double> displace_vec) {
        for (auto& callback : on_collisions_) {
            callback(other, displace_vec);
        }
    }

    //! Set collidable position to offset
    void SetCollidPositionOffset(Math::Vec2<double> offset) {
        collid_pos_offset_ = offset;
    }

   protected:
    Type type_;
    //! Callback function, defaults to do nothing
    std::vector<std::function<void(CollidableObject&, Math::Vec2<double>)>>
        on_collisions_;
    /*!
     * \brief Dimension of the rectangle that defines the boundary for collision
     * detection
     */
    std::pair<int, int> dimen_ {GetTextureDimen()};

    Math::Vec2<double> collid_pos_offset_ {0, 0};
};

#endif
