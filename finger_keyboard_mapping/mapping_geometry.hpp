#ifndef MAPPING_GEOMETRY_HPP
#define MAPPING_GEOMETRY_HPP

namespace finger_tracking {
struct Point {
    int x;
    int y;
};

struct Size {
    unsigned width;
    unsigned height;
};

namespace geo_literals {
struct xpos {
    int v;
};
struct ypos {
    int v;
};

consteval xpos operator"" _x(unsigned long long x) {
    return xpos{static_cast<int>(x)};
}
constexpr xpos operator*(int scalar, xpos x) {
    return xpos{scalar * x.v};
}

constexpr xpos operator+(xpos lhs, xpos rhs) {
    return xpos{lhs.v + rhs.v};
}

consteval ypos operator""_y(unsigned long long y) {
    return ypos{static_cast<int>(y)};
}

constexpr Point operator,(xpos x, ypos y) {
    return {.x = x.v, .y = y.v};
}

struct width {
    unsigned v;
};
struct height {
    unsigned v;
};

consteval width operator"" _w(unsigned long long w) {
    return {.v = static_cast<unsigned>(w)};
}
consteval height operator"" _h(unsigned long long h) {
    return {.v = static_cast<unsigned>(h)};
}
consteval Size operator,(width w, height h) {
    return {.width = w.v, .height = h.v};
}
} // namespace geo_literals

struct Rectangle {
    Point topLeft;
    Size  size;
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

#endif // MAPPING_GEOMETRY_HPP
