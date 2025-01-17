#ifndef ROW_COL_HPP
#define ROW_COL_HPP

namespace finger_tracking {
enum Row : std::uint8_t {
    Fn = 0,
    Top,
    Home,
    Bottom,
    Thumb
};

enum Column : std::uint8_t {
    IndexExt = 0,
    Index,
    Middle,
    Ring,
    Pinky,
    PinkyExt,
};
} // namespace finger_tracking

#endif // ROW_COL_HPP
