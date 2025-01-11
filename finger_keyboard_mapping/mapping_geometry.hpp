#ifndef MAPPING_GEOMETRY_HPP
#define MAPPING_GEOMETRY_HPP

namespace finger_tracking {
struct Point {
    int x;
    int y;

    bool  operator==(Point const& other) const = default;
    Point operator-(Point const& other) const { return {x - other.x, y - other.y}; }

    float sqrMagnitude() const { return x * x + y * y; }
};

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
