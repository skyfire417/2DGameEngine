#include "object/fallable_object.h"

double FallableObject::gravity_ {17};

void FallableObject::ReadConfiguration() {
    gravity_ = GetConfigProvider().GetRoot()["physics"]["gravity"].asDouble();
    assert(gravity_ > 0);
}
