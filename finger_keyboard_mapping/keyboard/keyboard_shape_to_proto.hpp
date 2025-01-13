#ifndef KEYBOARD_SHAPE_TO_PROTO_HPP
#define KEYBOARD_SHAPE_TO_PROTO_HPP

#include "finger_keyboard_mapping/keyboard/keyboard_shape.hpp"

#include <finger_keyboard_mapping/keyboard/keyboard_shape.pb.h>

namespace finger_tracking {

inline proto::KeyboardShape serializeKbShapeToProto(KeyboardShape const& shape) {

    proto::KeyboardShape output{};

    for (auto const& key : shape.keys()) {
        auto* k = output.add_keys();
        k->set_name(key.key.name);
        k->mutable_aabb()->set_x(key.aabb.topLeft.x);
        k->mutable_aabb()->set_y(key.aabb.topLeft.y);
        k->mutable_aabb()->set_width(key.aabb.size.width);
        k->mutable_aabb()->set_height(key.aabb.size.height);
    }

    return output;
}
} // namespace finger_tracking

#endif // KEYBOARD_SHAPE_TO_PROTO_HPP
