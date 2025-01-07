#include "finger_to_key_mapping.hpp"

namespace finger_tracking {

struct Key {};

struct KeyboardCanonicalShape {

    std::vector<Key> keys;
};

class KeyboardShape {
    KeyboardCanonicalShape m_shape;
    Corners                m_projectedCorners;

public:
    KeyboardShape(KeyboardCanonicalShape shape, Corners const& projectedCorners)
        : m_shape(std::move(shape))
        , m_projectedCorners(projectedCorners) {}

    Key keyAt(Point point) { return {}; }
};

void mapFingersToKeys(std::vector<Frame> frames, Corners corners) {}
} // namespace finger_tracking
