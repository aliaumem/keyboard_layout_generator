#include "layout_generator/keyboard_layout.hpp"

#include "voyager_shape.hpp"

namespace finger_tracking {
TargetKeyboardLayout azertyVoyagerLayout() {
    // clang-format off
    auto lowerKeys = std::array<Key, 52>{
        "Esc",  "&", "é", "\"",  "'", "(",    "-", "è", "_", "ç", "à",  ")",
        "CpLk", "a", "z", "e",  "r", "t",     "y", "u", "i", "o", "p",  "^",
         "<",   "q", "s", "d",  "f", "g",     "h", "j", "k", "l", "m",  "ù",
          "",   "w", "x", "c",  "v", "b",     "n", ",", ";", ":", "!", "Menu",
                             " ",  "Tab",     "BkSp", "\n"
    };
    auto lowerHeldKeys = std::array<Key, 52>{
        "", "",  "",  "",  "",  "",      "",  "",   "",  "",  "",  "",
        "", "",  "",  "",  "",  "",      "",  "",   "",  "",  "",  "",
        "", "Win",  "LAlt",  "LSft",     "LCtl", "LSft", "LAlt",  "Win", "",
        "", "",  "",  "",  "",  "",      "",  "", "",  "",  "",  "",
                           "",  "",      "", "RAlt"
    };

    auto upperKeys = std::array<Key, 52>{
        "",  "1", "2", "3",  "4", "5",     "6", "7", "8", "9", "0",  "+",
        "",  "A", "Z", "E",  "R", "T",     "Y", "U", "I", "O", "P",  "¨",
        ">", "Q", "S", "D",  "F", "G",     "H", "J", "K", "L", "M",  "%",
        "",  "W", "X", "C",  "V", "B",     "N", "?", ".", "/", "§",  "",
                              "",  "",     "", ""
    };
    auto altKeys = std::array<Key, 52>{
        "", "",  "~", "#", "{", "[",     "|", "`", "\\", "^", "@", "]",
        "", "",  "",  "€", "",  "",     "",  "",   "",  "",  "",  "*",
        "", "",  "",  "",  "",  "",      "",  "",   "",  "",  "$",  "",
        "", "",  "",  "",  "",  "",      "",  "",   "",  "",  "=",  "}",
                           "",  "",      "", ""
    };
    auto layer2Keys = std::array<Key, 52>{
        "", "",  "", "", "", "",         "",   "",     "",     "",   "", "",
        "", "",  "",  "", "",  "",       "",   "",   "UArr",   "",   "", "",
        "", "",  "",  "",  "",  "",      "", "LArr", "DArr", "RArr", "", "",
        "", "",  "",  "",  "",  "",      "",   "",     "",     "",   "", "",
                           "",  "",      "",   ""
    };
    // clang-format on

    std::vector<TargetKeyboardLayout::layer_type> layers{
        {lowerKeys, lowerHeldKeys},
        {upperKeys},
        {altKeys},
        {layer2Keys},
    };

    return TargetKeyboardLayout{
        voyagerShape(),
        std::move(layers),
        {
            LayerJumpKey{Key{"LSft"}, 1},
            LayerJumpKey{Key{"RAlt"}, 2},
            LayerJumpKey{Key{" "}, 3},
        }};
}
} // namespace finger_tracking
