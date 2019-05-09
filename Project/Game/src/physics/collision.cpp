#include "physics/collision.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

std::optional<Math::Vec2<double>> Collision::Collide(
    const Math::Rect<double>& from, const Math::Rect<double>& to) {
    auto axes = Collision::GetSeparatingAxes();
    Math::Vec2<double> smallest_displace_axis;
    double smallest_displace = std::numeric_limits<double>::max();
    for (const auto& axis : axes) {
        Math::LineSeg1<double> proj_from = Math::ProjectRectToVec(from, axis);
        Math::LineSeg1<double> proj_to = Math::ProjectRectToVec(to, axis);
        if (!Math::DoLineSegOverlap(proj_from, proj_to)) {
            // No collision happens
            return std::nullopt;
        } else {
            // If two projections overlap, save its information
            auto res = Collision::GetDisplacement(proj_to, proj_from);
            assert(res.has_value());
            double displace = res.value();
            if (std::abs(displace) < std::abs(smallest_displace)) {
                smallest_displace = displace;
                smallest_displace_axis = axis;
            }
        }
    }
    // Collision detected
    return Math::Scale(smallest_displace_axis, smallest_displace);
}

void Collision::ObserveCollision(CollidableObject::Type from_type,
                                 CollidableObject::Type to_type) {
    observed_types_[from_type].push_back(to_type);
}

void Collision::RemoveObservation(const CollidableObject::Type& from_type,
                                  const CollidableObject::Type& to_type) {
    auto found_from = observed_types_.find(from_type);
    if (found_from != observed_types_.end()) {
        auto found_tos = found_from->second;
        auto found_to = std::find(found_tos.begin(), found_tos.end(), to_type);
        if (found_to != found_tos.end()) {
            found_tos.erase(found_to);
        }
    }
}

void Collision::AddCollidableObject(std::shared_ptr<CollidableObject> obj) {
    observed_objects_[obj->CollidObjType()].push_back(obj);
}

void Collision::AddCollidableObject(
    std::vector<std::shared_ptr<CollidableObject>> objs) {
    for (auto& obj : objs) {
        observed_objects_[obj->CollidObjType()].push_back(obj);
    }
}

void Collision::RemoveCollidableObject(const CollidableObject& obj) {
    auto found_type = observed_objects_.find(obj.CollidObjType());
    if (found_type != observed_objects_.end()) {
        auto& found_objs = found_type->second;
        auto found_obj = std::find_if(
            found_objs.begin(), found_objs.end(),
            [&obj](const auto& obj_ptr) { return obj_ptr.get() == &obj; });
        if (found_obj != found_objs.end()) {
            found_objs.erase(found_obj);
        }
    }
}

void Collision::MarkCollidableObjectAsRemoved(const CollidableObject* obj) {
    observed_objects_to_remove_.push_back(obj);
}

void Collision::Update() {
    for (auto& [from_type, to_types] : observed_types_) {
        for (auto& to_type : to_types) {
            for (auto& from_object : observed_objects_[from_type]) {
                for (auto& to_object : observed_objects_[to_type]) {
                    auto res = Collision::Collide(from_object->CollidObjRect(),
                                                  to_object->CollidObjRect());
                    if (res.has_value()) {
                        Math::Vec2<double> displace_vec = res.value();
                        from_object->OnCollision(*to_object, displace_vec);
                        to_object->OnCollision(*from_object,
                                               Math::Scale(displace_vec, -1.0));
                    }
                }
            }
        }
    }

    for (auto& obj_ptr : observed_objects_to_remove_) {
        RemoveCollidableObject(*obj_ptr);
    }
    observed_objects_to_remove_.clear();
}

std::vector<Math::Vec2<double>> Collision::GetSeparatingAxes() {
    return {{0, 1}, {1, 0}};
}

std::optional<double> Collision::GetDisplacement(
    const Math::LineSeg1<double>& proj_to,
    const Math::LineSeg1<double>& proj_from) {
    if (proj_from.max < proj_to.max && proj_from.max > proj_to.min) {
        return proj_to.min - proj_from.max;
    }

    if (proj_to.max > proj_from.min && proj_to.max < proj_from.max) {
        return proj_to.max - proj_from.min;
    }

    if (Math::DoLineSegContain(proj_from, proj_to)) {
        double overlap = proj_to.max - proj_from.min;
        double diff_right = proj_from.max - proj_to.max;
        double diff_left = proj_to.min - proj_from.min;
        double extra_displace = diff_right;
        if (diff_left < diff_right) {
            extra_displace = diff_left;
            return overlap + extra_displace;
        } else {
            return -(overlap + extra_displace);
        }
    } else if (Math::DoLineSegContain(proj_to, proj_from)) {
        double overlap = proj_from.max - proj_to.min;
        double diff_right = proj_to.max - proj_from.max;
        double diff_left = proj_from.min - proj_to.min;
        double extra_displace = diff_right;
        if (diff_left < diff_right) {
            extra_displace = diff_left;
            return -(overlap + extra_displace);
        } else {
            return overlap + extra_displace;
        }
    }

    return std::nullopt;
}
