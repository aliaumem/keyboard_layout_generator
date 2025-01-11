#ifndef MAPPING_GEOMETRY_HPP
#define MAPPING_GEOMETRY_HPP
#include <optional>

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

constexpr xpos operator-(xpos x) {
    return {-x.v};
}

constexpr xpos operator+(xpos lhs, xpos rhs) {
    return xpos{lhs.v + rhs.v};
}

constexpr ypos operator-(ypos y) {
    return {-y.v};
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

struct FingerDesc {
    enum class Side {
        Left,
        Right,
    };
    enum class Finger {
        Thumb,
        Index,
        Middle,
        Ring,
        Pinky,
    };

    Side   side;
    Finger finger;

    FingerDesc(Side side, Finger finger)
        : side(side)
        , finger(finger) {}

    explicit FingerDesc(int value) {
        side   = value / 5 == 0 ? Side::Left : Side::Right;
        finger = [value] {
            switch (value % 5) {
            case 0: return Finger::Thumb;
            case 1: return Finger::Index;
            case 2: return Finger::Middle;
            case 3: return Finger::Ring;
            case 4: return Finger::Pinky;
            default: return Finger::Thumb;
            }
        }();
    }

    explicit operator int() const {
        return side == Side::Right ? 5 : 0 + [this] {
            switch (finger) {
            case Finger::Thumb: return 0;
            case Finger::Index: return 1;
            case Finger::Middle: return 2;
            case Finger::Ring: return 3;
            case Finger::Pinky: return 4;
            default: return 0;
            }
        }();
    }

    bool operator==(FingerDesc const&) const = default;
};

struct FingerRef {
    FingerDesc fingerDesc;
    Point      position;
    bool       operator==(FingerRef const& other) const = default;
};

struct BothHands {
    struct iterator {
        using value_type     = FingerRef;
        using reference_type = FingerRef;

        int              index = 0;
        BothHands const* hands;

        FingerRef operator*() const {
            Point position = [this]() {
                switch (index) {
                case 0: return hands->left->thumb;
                case 1: return hands->left->index;
                case 2: return hands->left->middle;
                case 3: return hands->left->ring;
                case 4: return hands->left->pinky;
                case 5: return hands->right->thumb;
                case 6: return hands->right->index;
                case 7: return hands->right->middle;
                case 8: return hands->right->ring;
                case 9: return hands->right->pinky;
                default: return Point{100, 100};
                }
            }();

            return {FingerDesc{index}, position};
        }
        iterator& operator++() {
            ++index;
            return *this;
        }
        bool operator==(iterator const& other) const {
            return hands == other.hands && index == other.index;
        }
    };

    std::optional<Hand> left;
    std::optional<Hand> right;

    [[nodiscard]] iterator begin() const {
        if (!left.has_value() && right.has_value())
            return {5, this};
        return {0, this};
    }

    [[nodiscard]] iterator end() const {
        if (right.has_value())
            return iterator{10, this};
        else if (left.has_value())
            return iterator{5, this};
        else
            return iterator{0, this};
    }

    [[nodiscard]] std::size_t size() const {
        return (left.has_value() ? 5 : 0) + (right.has_value() ? 5 : 0);
    }
};

struct Corners {
    Point topLeft;
    Point topRight;
    Point bottomRight;
    Point bottomLeft;
};
} // namespace finger_tracking

#endif // MAPPING_GEOMETRY_HPP
