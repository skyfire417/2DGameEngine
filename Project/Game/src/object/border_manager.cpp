#include "object/border_manager.h"
#include <tuple>

BorderManager::BorderManager() {
    std::vector<Border::Side> sides {Border::Side::kTop, Border::Side::kBottom,
                                     Border::Side::kRight, Border::Side::kLeft};
    for (auto& side : sides) {
        auto border = std::make_shared<Border>(side);
        border->SetOnCollisionFunc([](auto& o, auto v) {
            // Displaces the incoming object
            o.AdjustPos(Math::Scale(v, -1.0));
        });
        borders_[side] = border;
    }
}

void BorderManager::ResetPosition(const Math::Rect<int>& area) {
    std::vector<std::tuple<Border::Side, Math::Vec2<int>, int>> args {{
        {Border::Side::kTop, {area.x, area.y}, area.w},
        {Border::Side::kRight, {area.x + area.w, area.y}, area.h},
        {Border::Side::kBottom, {area.x, area.y + area.h}, area.w},
        {Border::Side::kLeft, {area.x, area.y}, area.h},
    }};
    for (auto& arg : args) {
        Border::Side side = std::get<0>(arg);
        borders_[side]->SetPos(std::get<1>(arg), std::get<2>(arg));
    }
    boundary_ = area;
}

std::vector<std::shared_ptr<CollidableObject>>
BorderManager::ShareBorderCollidableObjects() {
    std::vector<std::shared_ptr<CollidableObject>> res;
    res.resize(borders_.size());
    std::transform(borders_.begin(), borders_.end(), res.begin(),
                   [](auto& b) { return b.second; });
    return res;
}

void BorderManager::SetOnCollisionFunc(
    Border::Side side,
    std::function<void(CollidableObject&, Math::Vec2<double>)> callback) {
    borders_[side]->SetOnCollisionFunc(callback);
}
