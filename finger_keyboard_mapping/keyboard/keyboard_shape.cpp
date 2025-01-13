#include "keyboard_shape.hpp"

#include <numeric>

namespace finger_tracking {
Rectangle KeyboardShape::aabb() const {
    Rectangle total = std::accumulate(
        m_keys.begin(), m_keys.end(), Rectangle{}, [](Rectangle accu, KeyInSpace key) {
            auto br = key.aabb.bottomRight();
            return Rectangle{.topLeft = accu.topLeft,
                             .size    = {
                                    .width  = std::max(accu.size.width, br.x < 0 ? 0u : br.x),
                                    .height = std::max(accu.size.height, br.y < 0 ? 0u : br.y),
                             }};
        });
    return total;
}
} // namespace finger_tracking
