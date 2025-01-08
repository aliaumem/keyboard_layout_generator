#ifndef PRINT_HELPERS_HPP
#define PRINT_HELPERS_HPP

#include <ostream>

std::ostream& operator<<(std::ostream& os, finger_tracking::Point const& point) {
    return os << "(" << point.x << ", " << point.y << ")";
}
std::ostream& operator<<(std::ostream& os, finger_tracking::FingerDesc::Finger finger) {
    using enum finger_tracking::FingerDesc::Finger;
    switch (finger) {
    case Thumb: os << "Thumb"; break;
    case Index: os << "Index"; break;
    case Middle: os << "Middle"; break;
    case Ring: os << "Ring"; break;
    case Pinky: os << "Pinky"; break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, finger_tracking::FingerDesc::Side side) {
    return os << (side == finger_tracking::FingerDesc::Side::Left ? "Left" : "Right");
}

std::ostream& operator<<(std::ostream& os, finger_tracking::FingerDesc const& d) {
    return os << '{' << d.side << ", " << d.finger << ", " << d.position << '}';
}

template <typename T>
std::ostream& operator<<(std::ostream& os, std::optional<T> const& opt) {
    if (opt.has_value())
        os << opt.value();
    else
        os << "[nullopt]";
    return os;
}

#endif // PRINT_HELPERS_HPP
