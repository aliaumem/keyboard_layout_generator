#include "layout_generator/keyboard_layout.hpp"

#include "voyager_shape.hpp"

namespace finger_tracking {
TargetKeyboardLayout azertyVoyagerLayout() {
    // clang-format off
    auto lowerKeys = std::array<Key, 52>{
        "Esc",  "&", "é", "\"",  "'", "(",    "-", "è", "_", "ç", "à",  ")",
        "CpLk", "a", "z", "e",  "r", "t",     "y", "u", "i", "o", "p",  "^",
        "LSft", "q", "s", "d",  "f", "g",     "h", "j", "k", "l", "m",  "ù",
          "<",  "w", "x", "c",  "v", "b",     "n", ",", ";", ":", "!", "RCtl",
                           "Spc",  "Tab",     "BkSp", "Enter"
    };
    auto upperKeys = std::array<Key, 52>{
        "Esc",  "1", "2", "3",  "4", "5",     "6", "7", "8", "9", "0",  "+",
        "CpLk", "A", "Z", "E",  "R", "T",     "Y", "U", "I", "O", "P",  "¨",
        "LSft", "Q", "S", "D",  "F", "G",     "H", "J", "K", "L", "M",  "%",
          ">",  "W", "X", "C",  "V", "B",     "N", "?", ".", "/", "§", "RCtl",
                           "Spc",  "Tab",     "BkSp", "Enter"
    };
    auto altKeys = std::array<Key, 52>{
        "", "",  "~", "#", "{", "[",     "|", "`", "\\", "^", "@", "}",
        "", "",  "",  "€", "",  "T",     "",  "",   "",  "",  "",  "",
        "", "",  "",  "",  "",  "",      "",  "",   "",  "",  "",  "",
        "", "",  "",  "",  "",  "",      "",  "",   "",  "",  "",  "",
                           "",  "",      "", ""
    };
    // clang-format on

    std::vector<TargetKeyboardLayout::layer_type> layers{
        {lowerKeys},
        {upperKeys},
        {altKeys},
    };
    //

    return TargetKeyboardLayout{voyagerShape(), std::move(layers)};
}
} // namespace finger_tracking
