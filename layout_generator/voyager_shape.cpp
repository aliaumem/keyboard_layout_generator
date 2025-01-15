#include "voyager_shape.hpp"

#include "range/v3/view/enumerate.hpp"

namespace finger_tracking {
KeyboardShape<52> voyagerShape() {
    using namespace geo_literals;

    std::array<Rectangle, 52> layout{};

    std::array topRowOffsets{6, 6, 2, 0, 6, 8, 8, 6, 0, 2, 6, 6};
    std::array sideOffsets{0, 0, 0, 0, 0, 0, 60, 60, 60, 60, 60, 60};
    for (auto [column, offset] : topRowOffsets | ranges::views::enumerate) {
        for (int i = 0; i < 4; ++i) {
            layout[column * 4 + i] = Rectangle{
                (static_cast<int>(column) * 19_x + sideOffsets[column], i * 19_y + offset),
                (18_w, 18_h),
            };
        }
    }

    // left thumb cluster
    layout[48] = Rectangle{(123_x, 85_y), (18_w, 18_h)};
    layout[49] = Rectangle{(132_x, 91_y), (18_w, 28_h)};

    // right thumb cluster
    layout[50] = Rectangle{(165_x, 85_y), (18_w, 18_h)};
    layout[51] = Rectangle{(156_x, 91_y), (18_w, 28_h)};

    return KeyboardShape(layout);
}
} // namespace finger_tracking
