#ifndef MAPPING_GEOMETRY_HPP
#define MAPPING_GEOMETRY_HPP

namespace finger_tracking {
template <typename T>
struct Point_ {
    T x;
    T y;

    bool   operator==(Point_ const& other) const = default;
    Point_ operator-(Point_ const& other) const {
        return {static_cast<T>(x - other.x), static_cast<T>(y - other.y)};
    }
    template <typename U>
    Point_<U> cast() {
        return Point_<U>{static_cast<U>(x), static_cast<U>(y)};
    }
};
using Point   = Point_<int>;
using Point16 = Point_<std::int16_t>;

struct Size {
    unsigned width;
    unsigned height;
};

struct Rectangle {
    Point topLeft;
    Size  size;

    [[nodiscard]] Point center() const {
        return {topLeft.x + static_cast<int>(size.width) / 2,
                topLeft.y + static_cast<int>(size.height) / 2};
    }

    [[nodiscard]] Point bottomRight() const {
        return {topLeft.x + static_cast<int>(size.width),
                topLeft.y + static_cast<int>(size.height)};
    }

    [[nodiscard]] bool contains(Point const& p) const {
        auto delta = p - topLeft;
        return 0 <= delta.x && delta.x <= size.width && 0 <= delta.y && delta.y <= size.height;
    }
};

struct Hand {
    Point thumb;
    Point index;
    Point middle;
    Point ring;
    Point pinky;
};

struct Corners {
    Point topLeft;
    Point topRight;
    Point bottomRight;
    Point bottomLeft;
};
} // namespace finger_tracking

#include "finger_keyboard_mapping/mapping_geometry_literals.inl"

#endif // MAPPING_GEOMETRY_HPP
