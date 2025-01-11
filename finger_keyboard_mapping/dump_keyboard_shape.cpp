#include "finger_to_key_mapping.hpp"
#include "finger_keyboard_mapping/keyboard_shape_to_proto.hpp"

#include <fstream>

int main(int argc, char* argv[]) {
    auto shape = finger_tracking::KeyboardShape::defaultShape();
    auto proto = finger_tracking::serializeKbShapeToProto(shape);

    std::ofstream out(argv[1], std::ios::binary);
    proto.SerializeToOstream(&out);

    return 0;
}
