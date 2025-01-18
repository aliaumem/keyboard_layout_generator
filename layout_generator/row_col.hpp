#ifndef ROW_COL_HPP
#define ROW_COL_HPP

#include <cstdint>

namespace finger_tracking {
enum class Row : std::uint8_t {
    Fn = 0,
    Top,
    Home,
    Bottom,
    Thumb
};

inline int operator-(Row lhs, Row rhs) {
    return static_cast<int>(lhs) - static_cast<int>(rhs);
}

enum class Column : std::uint8_t {
    IndexExt = 0,
    Index,
    Middle,
    Ring,
    Pinky,
    PinkyExt,
};
} // namespace finger_tracking

#endif // ROW_COL_HPP
