#include "finger_to_key_mapping.hpp"

#include "keyboard_timeline.hpp"
#include "scancode_key_map.hpp"

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

std::optional<FingerRef> KeyboardShape::closestFinger(Key key, BothHands const& hands) const {
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

KeyboardTimeline mapFingersToKeys(
    std::vector<Frame> const& frames, std::vector<KeyEvent> const& keyEvents,
    KeyboardShape const& shape, ScancodeKeyMap const& scancodeKeyMap) {
    using std::chrono::milliseconds;

    KeyboardTimeline::Builder builder;

    auto prevEvent = keyEvents.begin();
    for (auto const& frame : frames) {
        while (prevEvent != keyEvents.end() && prevEvent->timestamp < frame.timestamp) {
            auto key         = scancodeKeyMap.scanCodeToKey(prevEvent->code);
            auto maybeFinger = shape.closestFinger(key, frame.hands);
            if (maybeFinger.has_value()) {
                if (prevEvent->isPressed)
                    builder.pressed(key, maybeFinger->fingerDesc);
                else
                    builder.released(key);
            }
            ++prevEvent;
        }

        builder.nextFrame();
    }

    return builder.build();
}
} // namespace finger_tracking
