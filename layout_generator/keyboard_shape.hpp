#ifndef KEYBOARD_SHAPE_HPP
#define KEYBOARD_SHAPE_HPP

#include "finger_keyboard_mapping/hands/hand_side.hpp"
#include "finger_keyboard_mapping/mapping_geometry.hpp"

#include <array>

namespace finger_tracking {

struct KeyRef {
    HandSide     side;
    std::uint8_t row;
    std::uint8_t col;
};

template <size_t N>
struct KeyboardShape {
    std::array<Rectangle, N> keys;

    [[nodiscard]] std::uint8_t indexInLayer(HandSide side, std::uint8_t row,
                                            std::uint8_t col) const {
        auto   split     = row == 4 ? 2 : 6;
        size_t linearCol = side == HandSide::Left ? split - 1 - col : col + split;
        return row * 12 + linearCol;
    }

    [[nodiscard]] KeyRef atIndex(std::uint8_t index) const {
        std::uint8_t const row       = index / 12;
        std::uint8_t const linearCol = index % 12;

        int const split = row == 4 ? 2 : 6;

        auto const side = linearCol < split ? HandSide::Left : HandSide::Right;
        std::uint8_t const col  = side == HandSide::Left ? split - 1 - linearCol : linearCol - split;
        return {side, row, col};
    }
};
} // namespace finger_tracking
#endif // KEYBOARD_SHAPE_HPP
