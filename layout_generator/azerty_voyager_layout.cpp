#include "layout_generator/keyboard_layout.hpp"

#include "voyager_shape.hpp"

namespace finger_tracking {
TargetKeyboardLayout azertyVoyagerLayout() {
    // clang-format off
    auto lowerKeys = std::array<Key, 52>{
        "Esc", "&", "é", "\"",  "'", "(",    "-", "è", "_", "ç", "à",  ")",
         "",   "a", "z", "e",  "r", "t",     "y", "u", "i", "o", "p",  "^",
         "<",  "q", "s", "d",  "f", "g",     "h", "j", "k", "l", "m",  "ù",
       "Lay3", "w", "x", "c",  "v", "b",     "n", ",", ";", ":", "!", "Menu",
                             " ",  "Tab",    "BkSp", "\n"
    };
    auto layer0Mask = std::array<bool, 52>{
        true,  false, false, false, false, false,    false, false, false, false, false, false,
        false, false, false, false, false, false,    false, false, false, false, false, false,
        false, false, false, false, false, false,    false, false, false, false, false, false,
        true,  false, false, false, false, false,    false, false, false, false, false, true,
                                      true, true,    true, true
    };
    auto lowerHeldKeys = std::array<Key, 52>{
        "", "",  "",  "",  "",  "",      "",  "",   "",  "",  "",  "",
        "", "",  "",  "",  "",  "",      "",  "",   "",  "",  "",  "",
        "", "Win",  "LAlt",  "LSft", "LCtl", "", "",     "LCtl", "LSft", "LAlt",  "Win", "",
        "", "",  "",  "",  "",  "",      "",  "", "",  "",  "",  "",
                           "",  "",      "", "RAlt"
    };
    auto sysKeys = std::array<Key, 52>{
        "", "",  "", "", "", "",         "",   "",     "",     "",   "", "",
        "", "",  "",  "", "",  "",       "",   "",   "UArr",   "",   "", "",
        "", "",  "",  "",  "",  "",      "", "LArr", "DArr", "RArr", "", "",
        "", "",  "",  "",  "",  "",      "",   "",     "",     "",   "", "",
                           "",  "",      "",   ""
    };
    auto numpadSymbolsKeys = std::array<Key, 52>{
      "F1", "F2", "F3", "F4", "F5", "F6",    "F7", "F8",  "F9", "F10", "F11", "F12",
        "²", "$", "#",  "+",  "{",  "}",     "*",  "7",  "8",   "9",   "-",   "",
        "",  "",  "-",  ">",  "(",  ")",     "/",  "4",  "5",   "6",   "+",   "",
        "",  "",  "",   "",   "[",  "]",     "0",  "1",  "2",   "3",   "=",   "",
                                "",  "",     "",   ""
    };
    auto extraLayerKeys = std::array<Key, 52>{
        "", "", "", "", "", "",     "", "", "", "", "", "",
        "", "", "", "", "", "",     "", "", "", "", "", "",
        "", "", "", "", "", "",     "", "", "", "", "", "",
        "", "", "", "", "", "",     "", "", "", "", "", "",
                       "",  "",     "",   ""
    };
    // clang-format on

    std::array<bool, 52> fullyLockedMask;
    std::fill_n(fullyLockedMask.begin(), fullyLockedMask.size(), true);
    std::array<bool, 52> fullyUnlockedMask;
    std::fill_n(fullyUnlockedMask.begin(), fullyUnlockedMask.size(), false);

    std::vector<TargetKeyboardLayout::layer_type> layers{
        {lowerKeys, layer0Mask, lowerHeldKeys},
        {numpadSymbolsKeys, fullyLockedMask},
        {extraLayerKeys, fullyUnlockedMask},
    };

    return TargetKeyboardLayout{
        voyagerShape(),
        std::move(layers),
        {
            LayerJumpKey{Key{"Tab"}, 1},
            LayerJumpKey{Key{"Lay3"}, 2},
        }};
}
} // namespace finger_tracking
