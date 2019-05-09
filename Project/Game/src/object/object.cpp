#include "object/object.h"

void Object::Render(Math::Vec2<double> pos, int ms_passed) {
    GetImageProvider().RenderImage(image_id(), pos);
}

std::string Object::to_string() const {
    char buf[100];
    snprintf(buf, sizeof(buf),
             "[%s {pos: (%.2f, %.2f), rect: (%d, %d), vel: (%.2f, %.2f)}]",
             GetName().c_str(), pos_.x, pos_.y, GetTextureDimen().first,
             GetTextureDimen().second, vel_.x, vel_.y);
    return buf;
}
