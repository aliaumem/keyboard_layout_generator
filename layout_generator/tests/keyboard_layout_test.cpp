#include "layout_generator/voyager_shape.hpp"
#include "layout_generator/keyboard_layout.hpp"

#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"

#include <format>

std::ostream& operator<<(std::ostream& os, finger_tracking::Key const& value) {
    return os << std::format("{}", value);
}

#include "catch2/catch_test_macros.hpp"

using namespace finger_tracking;

SCENARIO("Layout is iterable") {
    // clang-format off
    auto keys = std::array<Key, 52>{
        "Esc",  "&", "é", "\"",  "'", "(",    "-", "è", "_", "ç", "à",  ")",
        "CpLk", "a", "z", "e",  "r", "t",     "y", "u", "i", "o", "p",  "^",
        "LSft", "q", "s", "d",  "f", "g",     "h", "j", "k", "l", "m",  "ù",
          "<",  "w", "x", "c",  "v", "b",     "n", "," ,";", ":", "!", "RCtl",
                        "Spc",  "Tab",          "BkSp", "Enter"
    };
    // clang-format on

    KeyboardLayer  layerNormal{keys};
    KeyboardLayout layout(voyagerShape(), std::vector{layerNormal});

    auto it = layout.begin();
    CHECK(it != layout.end());
    CHECK(std::distance(it, layout.end()) == 52);

    CHECK((*++it).first == Key{"&"});
}
