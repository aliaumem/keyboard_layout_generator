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

    KeyboardLayer layerNormal{keys};
    // KeyboardLayer  shiftedLayer{shiftedKeys};
    KeyboardLayout layout(voyagerShape(), std::vector{layerNormal}, {});

    auto it = layout.begin();
    CHECK(it != layout.end());
    CHECK(std::distance(it, layout.end()) == 52);
    CHECK(std::distance(layout.layerBegin(0), layout.layerEnd(0)) == 52);

    CHECK((*(++it)) == LayoutKeyRef{LayerId::defaultLayer, HandSide::Left, Row::Fn, Column::Pinky});

    CHECK(toShiftedKey(Key{"z"}).name == "Z");
    CHECK(toShiftedKey(Key{""}).name == "");
    CHECK(toShiftedKey(Key{"°"}).name == "");
    CHECK(toShiftedKey(Key{"<"}).name == ">");
    CHECK(toShiftedKey(Key{"^"}).name == "¨");
    CHECK(toShiftedKey(Key{"a"}).name == "A");
}
