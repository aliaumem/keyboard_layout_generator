#ifndef KEY_SEQUENCE_PRINTER_HPP
#define KEY_SEQUENCE_PRINTER_HPP

#include "finger_keyboard_mapping/keyboard/keyboard_shape.hpp"

#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"
#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"

#include <chrono>
#include <iostream>

namespace finger_tracking {
struct KeySequencePrinter {
    KeyboardShape             shape = KeyboardShape::defaultShape();
    std::chrono::milliseconds lastTimestamp{};
    Point                     prevLeft{};
    Point                     prevRight{};

    void print(auto const& chunk) {
        std::cout << "-----------" << std::endl;
        for (auto const& state : chunk) {
            auto delta       = state.timestamp - lastTimestamp;
            lastTimestamp    = state.timestamp;
            auto  pressedKey = state.pressedKeys.front();
            auto& prev = pressedKey.second.side == FingerDesc::Side::Left ? prevLeft : prevRight;
            auto  pos  = shape.keyAABB(pressedKey.first);
            auto  vec  = prev - pos.center();
            prev       = pos.center();

            std::cout << fmt::format(
                "{: <6} {}\t{}  \t{}", delta, pressedKey.first, pressedKey.second, vec)
                      << std::endl;
        }
    }
};
} // namespace finger_tracking

#endif // KEY_SEQUENCE_PRINTER_HPP
