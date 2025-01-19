#include "layout_generator/voyager_shape.hpp"
#include "layout_generator/keyboard_layout.hpp"

#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"
#include "layout_generator/layout_print_helpers.hpp"

#include "catch_string_helper.hpp"

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
    auto shiftedKeys = std::array<Key, 52>{
        "",  "1", "2", "3", "4", "5",     "6", "7", "8", "9", "0", "°",
        "",  "A", "Z", "E", "R", "T",     "Y", "U", "I", "O", "P", "¨",
        "",  "Q", "S", "D", "F", "G",     "H", "J", "K", "L", "M", "%",
        ">", "W", "X", "C", "V", "B",     "N", "?" ,".", "/", "§", "",
                        "",  "",          "", ""
    };
    // clang-format on

    KeyboardLayer  layerNormal{keys, {}, {}, {uint8_t{1}}};
    KeyboardLayer  shiftedLayer{shiftedKeys};
    KeyboardLayout layout(voyagerShape(), std::vector{layerNormal, shiftedLayer}, {});

    auto it = layout.begin();
    CHECK(it != layout.end());
    CHECK(std::distance(it, layout.end()) == 52 * 2);
    CHECK(std::distance(layout.layerBegin(0), layout.layerEnd(0)) == 52);
    CHECK(std::distance(layout.layerBegin(1), layout.layerEnd(1)) == 52);

    CHECK((*(++it)) == LayoutKeyRef{0ull, HandSide::Left, Row::Fn, Column::Pinky});

    CHECK(layout.areRelatedLayers(0, 1));
}
