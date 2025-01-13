#include "closest_finger_mapping.hpp"

namespace finger_tracking {
MaybeFinger closestFinger(KeyboardShape const& shape, Key key, BothHands const& hands) {
    if (hands.size() == 0)
        return MaybeFinger::nullopt();

    auto it = std::find_if(shape.keys().begin(), shape.keys().end(), [&key](KeyInSpace const& k) {
        return k.key.name == key.name;
    });
    if (it == shape.keys().end())
        return MaybeFinger::nullopt();

    auto toDistance = [it](FingerRef const& finger) -> int {
        auto delta = it->aabb.center() - finger.position;
        return delta.x * delta.x + delta.y * delta.y;
    };

    auto distances      = hands | std::views::transform(toDistance);
    auto fingerDistance = std::ranges::min_element(distances);

    auto dim = std::max(it->aabb.size.width, it->aabb.size.height) * 3 / 2;
    if (*fingerDistance <= dim * dim)
        return {*fingerDistance.base(), *fingerDistance};
    return MaybeFinger::nullopt();
}
} // namespace finger_tracking
