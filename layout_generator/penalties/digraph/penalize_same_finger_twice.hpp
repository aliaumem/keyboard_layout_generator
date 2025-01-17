#ifndef PENALIZE_SAME_FINGER_TWICE_HPP
#define PENALIZE_SAME_FINGER_TWICE_HPP

#include "layout_generator/penalties/ngraphs.hpp"

namespace finger_tracking::penalties {
float penalizeSameFingerTwice(Digraph const& digraph) {
    auto current  = digraph.current();
    auto previous = digraph.prev1();

    bool isSameFinger = current.fingerDesc() == previous.fingerDesc();

    return current.keyRef != previous.keyRef && isSameFinger ? 4.f : 0.f;
}
} // namespace finger_tracking::penalties
#endif // PENALIZE_SAME_FINGER_TWICE_HPP
