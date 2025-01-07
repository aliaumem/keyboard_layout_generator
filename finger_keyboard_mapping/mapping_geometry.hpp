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
