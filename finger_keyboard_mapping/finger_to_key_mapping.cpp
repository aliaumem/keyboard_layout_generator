#include "finger_to_key_mapping.hpp"

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

std::optional<FingerRef> KeyboardShape::closestFinger(Key key, BothHands const& hands) {
    if (hands.size() == 0)
        return std::nullopt;

    auto it = std::find_if(m_keys.begin(), m_keys.end(), [&key](KeyInSpace const& k) {
        return k.key.name == key.name;
    });
    if (it == m_keys.end())
        return std::nullopt;

    auto fingerIt
        = std::min_element(hands.begin(), hands.end(), [it](FingerRef lhs, FingerRef rhs) {
              return (it->aabb.center() - lhs.position).sqrMagnitude()
                   < (it->aabb.center() - rhs.position).sqrMagnitude();
          });

    return *fingerIt;
}

void mapFingersToKeys(std::vector<Frame> frames) {
    auto kbShape = KeyboardShape::defaultShape();
}
} // namespace finger_tracking
