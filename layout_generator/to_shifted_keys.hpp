#ifndef TO_SHIFTED_KEYS_HPP
#define TO_SHIFTED_KEYS_HPP

#include "finger_keyboard_mapping/keyboard/key.hpp"

#include "absl/container/flat_hash_map.h"

namespace finger_tracking {
namespace {
using kp_t = std::pair<Key, Key>;
absl::flat_hash_map<Key, Key> symbols{
    kp_t{"&", "1"},
    kp_t{"é", "2"},
    kp_t{"\"", "3"},
    kp_t{"'", "4"},
    kp_t{"(", "5"},
    kp_t{"-", "6"},
    kp_t{"è", "7"},
    kp_t{"_", "8"},
    kp_t{"ç", "9"},
    kp_t{"à", "0"},
    kp_t{")", "°"},
    // clang-format off
    kp_t{"^", "¨"
              ""}, // There seems to be a bug with this character, this is the best I could get
    // clang-format on
    kp_t{"ù", "%"},
    kp_t{"!", "§"},
    kp_t{",", "?"},
    kp_t{";", "."},
    kp_t{":", "/"},
    kp_t{"<", ">"},
};
constexpr std::string_view upperCaseLetters{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
} // namespace

inline Key toShiftedKey(Key key) {
    if (!key.isValid()) return Key::invalid;

    char value = key.name.front();
    if (value >= 'a' && value <= 'z')
        return Key{upperCaseLetters.substr(value - 'a', 1)};

    auto it = symbols.find(key);
    return it != symbols.end() ? it->second : Key::invalid;
}

namespace {
absl::flat_hash_map<Key, Key> altKeys{
    kp_t{"\"", "~"}, kp_t{"-", "|"}, kp_t{"è", "`"},
    kp_t{"_", "\\"}, kp_t{"à", "@"}, kp_t{"e", "€"},
};
} // namespace

inline Key toAltKey(Key key) {
    auto it = altKeys.find(key);
    return it != altKeys.end() ? it->second : Key::invalid;
}

} // namespace finger_tracking

#endif // TO_SHIFTED_KEYS_HPP
