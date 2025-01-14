#include "finger_keyboard_mapping/keyboard/scancode_key_map.hpp"

std::ostream& operator<<(std::ostream& os, finger_tracking::Key key) {
    return os << key.name;
}

#include "catch2/catch_test_macros.hpp"

using finger_tracking::Key;
using finger_tracking::KeyCode;
using finger_tracking::ScancodeKeyMap;

SCENARIO("The ScancodeKeyMap maps scancodes to key names") {
    ScancodeKeyMap map{};
    CHECK(map.scanCodeToKey(KeyCode{0x1D, true}) == Key{"RCtl"});
    CHECK(map.scanCodeToKey(KeyCode{0x56, false}) == Key{"<"});
    CHECK(map.scanCodeToKey(KeyCode{0x38, true}) == Key{"RAlt"});
}
