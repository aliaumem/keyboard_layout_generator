#ifndef FINGER_TO_KEY_MAP_HPP
#define FINGER_TO_KEY_MAP_HPP

#include "finger_keyboard_mapping/hands/finger_desc.hpp"

#include <array>

namespace finger_tracking {
struct FingerToKeyMap {
    std::array<std::array<std::uint16_t, 10>, 62> fingerKeyMap{};
    std::uint16_t& operator[](std::pair<std::size_t, FingerDesc> index) {
        return fingerKeyMap[index.first][static_cast<int>(index.second)];
    }
    std::uint16_t const& operator[](std::pair<std::size_t, FingerDesc> index) const {
        return fingerKeyMap[index.first][static_cast<int>(index.second)];
    }

    [[nodiscard]] auto begin() const { return fingerKeyMap.begin(); }
    [[nodiscard]] auto end() const { return fingerKeyMap.end(); }
};
} // namespace finger_tracking

#endif // FINGER_TO_KEY_MAP_HPP
