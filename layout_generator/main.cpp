#include "voyager_shape.hpp"
#include "finger_keyboard_mapping/keyboard/keyboard_shape_to_proto.hpp"

#include <fstream>

using namespace finger_tracking;

int main() {

    // clang-format off
    auto keys = static_vector<Key, 52>{std::array<Key, 52>{
        "Esc",  "&", "é", "\"", "'", "(",    "-", "è", "_", "ç", "à", ")", //
        "CpLk", "a", "z", "e", "r", "t",     "y", "u", "i", "o", "p", "^", //
        "LSft", "q", "s", "d", "f", "g",     "h", "j", "k", "l", "m", "ù", //
        "<",    "w", "x", "c", "v", "b",     "n", "," ,";", ":", "!", "RCtl", //
                        "Spc", "Tab",            "BkSp", "Enter"
    }};
    // clang-format on

    auto shape = voyagerShape(keys);

    auto          proto = serializeKbShapeToProto(shape);
    std::ofstream out{"C:/Users/aliau/Downloads/voyager_shape.binarypb", std::ios::binary};
    proto.SerializeToOstream(&out);
    proto.PrintDebugString();

    return 0;
}
