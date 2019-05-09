#ifndef PLATFORMER_PHYSICS_COLLISION_H
#define PLATFORMER_PHYSICS_COLLISION_H

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <utility>
#include "SDL2pp/Rect.hh"
#include "object/collidable_object.h"
#include "object/object.h"
#include "tinymath.h"

//! Does calculation of collision between objects
/*!
 * An object considered as an Axis Aligned Bounding Box (AABB).
 */
class Collision {
   public:
    /*!
     * \brief Determines if two boxes collide, and if they collide, returns a
     *        displacement vector as a response
     *
     * Using Separating Axis Theorem (SAT) to detect collision and
     * resolve collision. It first does a quick check of whether
     * collision happens and if not, it shortcuts and returns `std::nullopt`.
     */
    std::optional<Math::Vec2<double>> Collide(const Math::Rect<double>& from,
                                              const Math::Rect<double>& to);
    /*!
     * \brief Observes collision between two types of collision objects.
     *
     * Be sure to add collision objects.
     */
    void ObserveCollision(CollidableObject::Type from_type,
                          CollidableObject::Type to_type);
    //! Observes collision between two types of collision objects.
    void RemoveObservation(const CollidableObject::Type& from_type,
                           const CollidableObject::Type& to_type);
    /*!
     * \brief Adds collision objects to observe.
     *
     * Only collision of objects of registered types will be observed.
     * Order is honored. When collision happens, callback function of objects
     * (of same type) added first will be called first.
     */
    void AddCollidableObject(std::shared_ptr<CollidableObject> obj);
    //! Adds a list of collision objects to observe
    void AddCollidableObject(
        std::vector<std::shared_ptr<CollidableObject>> objs);
    //! Removes an object from observation list
    /*!
     */
    void RemoveCollidableObject(const CollidableObject& obj);
    /*!
     * \brief Removes an object from observation list the next time `Update()`
     *        is called.
     * The object will not be removed right away until the next `Update()`
     * call. This is to prevent mutation of the container of the observed
     * objects being changed during `Update()`.
     */
    void MarkCollidableObjectAsRemoved(const CollidableObject* obj);
    /*!
     * Update collision.
     */
    void Update();

   private:
    //! Returns normalized separating axes required by SAT algorithm
    /*!
     * Since we only use AABBs without rotating them, we only need
     * two axes.
     */
    std::vector<Math::Vec2<double>> GetSeparatingAxes();

    //! Calculates displacement to eject a penetrating object from another
    std::optional<double> GetDisplacement(
        const Math::LineSeg1<double>& proj_to,
        const Math::LineSeg1<double>& proj_from);

    std::unordered_map<CollidableObject::Type,
                       std::vector<CollidableObject::Type>>
        observed_types_;
    std::unordered_map<CollidableObject::Type,
                       std::vector<std::shared_ptr<CollidableObject>>>
        observed_objects_;
    std::vector<const CollidableObject*> observed_objects_to_remove_;
};

#endif
