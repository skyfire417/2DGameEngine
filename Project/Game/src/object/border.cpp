#include "object/border.h"

void Border::SetPos(Math::Vec2<int> start_point, int length) {
    switch (side_) {
        case Border::Side::kTop:
            Object::SetPos(start_point.x, start_point.y - Border::kDepth);
            SetCollidObjDimen(length, Border::kDepth);
            break;
        case Border::Side::kRight:
            Object::SetPos(start_point.x, start_point.y);
            SetCollidObjDimen(Border::kDepth, length);
            break;
        case Border::Side::kBottom:
            Object::SetPos(start_point.x, start_point.y);
            SetCollidObjDimen(length, Border::kDepth);
            break;
        case Border::Side::kLeft:
            Object::SetPos(start_point.x - Border::kDepth, start_point.y);
            SetCollidObjDimen(Border::kDepth, length);
            break;
    }
}
