#include "finger_keyboard_mapping/finger_to_key_mapping.hpp"

#include <array>

namespace finger_tracking {
KeyboardShape KeyboardShape::defaultShape() {
    std::vector<KeyInSpace> keys;
    using namespace geo_literals;
    keys.reserve(62);
    // First row
    std::array<std::string_view, 13> firstRowNames{
        "²", "&", "é", "\"", "'", "(", "-", "è", "_", "ç", "à", ")", "="};
    for (int i = 0; i < 13; ++i) {
        keys.emplace_back(Key{firstRowNames[i]}, Rectangle{(i * 19_x, 0_y), (18_w, 18_h)});
    }
    keys.emplace_back(Key{"BkSp"}, Rectangle{(247_x, 0_y), (35_w, 18_h)});

    // Second row
    keys.emplace_back(Key{"T"}, Rectangle{(0_x, 19_y), (26_w, 18_h)});
    std::array<std::string_view, 12> secondRowNames{
        "a", "z", "e", "r", "t", "y", "u", "i", "o", "p", "^", "$"};
    for (int i = 0; i < 12; ++i) {
        keys.emplace_back(Key{secondRowNames[i]}, Rectangle{(27_x + i * 19_x, 19_y), (18_w, 18_h)});
    }

    // Third row
    keys.emplace_back(Key{"C"}, Rectangle{(0_x, 38_y), (31_w, 18_h)});
    std::array<std::string_view, 12> thirdRowNames{
        "q", "s", "d", "f", "g", "h", "j", "k", "l", "m", "ù", "*"};
    for (int i = 0; i < 12; ++i) {
        keys.emplace_back(Key{thirdRowNames[i]}, Rectangle{(32_x + i * 19_x, 38_y), (18_w, 18_h)});
    }

    // Fourth row
    keys.emplace_back(Key{"1"}, Rectangle{(0_x, 57_y), (22_w, 18_h)});
    std::array<std::string_view, 11> fourthRowNames{
        "<", "w", "x", "c", "v", "b", "n", ",", ";", ":", "!"};
    for (int i = 0; i < 11; ++i) {
        keys.emplace_back(Key{fourthRowNames[i]}, Rectangle{(23_x + i * 19_x, 57_y), (18_w, 18_h)});
    }
    keys.emplace_back(Key{"2"}, Rectangle{(232_x, 57_y), (51_w, 18_h)});

    // Fifth row
    keys.emplace_back(Key{"LCtl"}, Rectangle{(0_x, 76_y), (22_w, 18_h)});
    keys.emplace_back(Key{"Win"}, Rectangle{(23_x, 76_y), (22_w, 18_h)});
    keys.emplace_back(Key{"LAlt"}, Rectangle{(46_x, 76_y), (22_w, 18_h)});
    keys.emplace_back(Key{"Spc"}, Rectangle{(69_x, 76_y), (120_w, 18_h)});
    keys.emplace_back(Key{"RAlt"}, Rectangle{(190_x, 76_y), (22_w, 18_h)});
    keys.emplace_back(Key{"Fn"}, Rectangle{(213_x, 76_y), (22_w, 18_h)});
    keys.emplace_back(Key{"Menu"}, Rectangle{(237_x, 76_y), (22_w, 18_h)});
    keys.emplace_back(Key{"RCtl"}, Rectangle{(260_x, 76_y), (22_w, 18_h)});

    // enter
    keys.emplace_back(Key{"Enter"}, Rectangle{(261_x, 19_y), (22_w, 37_h)});

    return KeyboardShape(std::move(keys));
}
} // namespace finger_tracking
