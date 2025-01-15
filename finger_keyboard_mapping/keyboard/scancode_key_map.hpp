#ifndef SCANCODE_KEY_MAP_HPP
#define SCANCODE_KEY_MAP_HPP

#include "finger_keyboard_mapping/static_vector.hpp"
#include "finger_keyboard_mapping/keyboard/key.hpp"

#include <chrono>

namespace finger_tracking {

struct KeyCode {
    int  scancode;
    bool isE0 = false;
    bool isE1 = false;

    bool operator==(KeyCode const& rhs) const = default;
    bool operator<(KeyCode const other) const {
        return scancode < other.scancode || (scancode == other.scancode && isE0 < other.isE0)
            || (scancode == other.scancode && isE0 == other.isE0 && isE1 < other.isE1);
    }
};

struct KeyEvent {
    std::chrono::milliseconds timestamp{};
    bool                      isPressed = false;
    KeyCode                   code;
};

struct ScancodeKeyMap {
    static_vector<KeyCode, 62> scancodes;
    static_vector<Key, 62>     keys;

    ScancodeKeyMap();

    [[nodiscard]] Key scanCodeToKey(KeyCode const& key) const {
        auto it = std::lower_bound(scancodes.begin(), scancodes.end(), key);
        if (it == scancodes.end() || it->scancode != key.scancode)
            return Key::invalid;

        return keys[std::distance(scancodes.begin(), it)];
    }

    size_t keyIndex(Key key) const {
        auto it = std::find(keys.begin(), keys.end(), key);
        if (it == keys.end() || *it != key) {
            if (key.isValid())
                throw std::out_of_range(std::format(
                    "key '{}' index out of range (found {} instead)", key.name, it->name));
            else
                throw std::invalid_argument("Trying to look up an invalid key");
        }
        return it - keys.begin();
    }
};

} // namespace finger_tracking

#endif // SCANCODE_KEY_MAP_HPP
