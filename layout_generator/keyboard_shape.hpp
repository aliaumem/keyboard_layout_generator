#ifndef KEYBOARD_SHAPE_HPP
#define KEYBOARD_SHAPE_HPP

#include "layout_generator/layout_key_ref.hpp"
#include "finger_keyboard_mapping/mapping_geometry.hpp"
#include "finger_keyboard_mapping/hands/finger_desc.hpp"

#include <array>

namespace finger_tracking {

template <size_t N>
struct KeyboardShape {
    std::array<Rectangle, N> keys;

    [[nodiscard]] std::uint8_t indexInLayer(KeyRef keyRef) const {
        auto [side, row, col] = keyRef;
        auto   split          = row == 4 ? 2 : 6;
        size_t linearCol      = side == HandSide::Left ? split - 1 - col : col + split;
        return row * 12 + linearCol;
    }

    [[nodiscard]] KeyRef atIndex(std::uint8_t index) const {
        std::uint8_t const row       = index / 12;
        std::uint8_t const linearCol = index % 12;

        int const split = row == 4 ? 2 : 6;

        auto const         side = linearCol < split ? HandSide::Left : HandSide::Right;
        std::uint8_t const col = side == HandSide::Left ? split - 1 - linearCol : linearCol - split;
        return {side, row, col};
    }

    [[nodiscard]] Finger fingerFor(KeyRef keyRef) const {
        switch (keyRef.col) {
        case 0: [[fallthrough]];
        case 1: return Finger::Index;
        case 2: return Finger::Middle;
        case 3: return Finger::Ring;
        default:
        case 4: return Finger::Pinky;
        }
    }
};
} // namespace finger_tracking
#endif // KEYBOARD_SHAPE_HPP
