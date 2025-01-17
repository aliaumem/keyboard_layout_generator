#ifndef PENALIZE_BASE_PENALTY_HPP
#define PENALIZE_BASE_PENALTY_HPP

#include "layout_generator/penalties/ngraphs.hpp"

namespace finger_tracking::penalties {
inline float penalizeBaseCost(KeyPress const& current, KeyPress const& /*previous*/) {
    // clang-format off
    std::array<float, 26> costs {
        /*4.f, 5.f, 4.f, 3.f, 3.f, 4.f, */    4.f, 3.f, 3.f, 4.f, 5.f, 4.f,
        /*3.f, 3.f, 1.f, 1.f, 1.5, 3.f, */    3.f, 1.5, 1.f, 1.f, 3.f, 3.f,
        /*2.f, 0.5, 0.5, 0.f, 0.f, 1.5, */    1.5, 0.f, 0.f, 0.5, 0.5, 2.f,
        /*4.f, 2.f, 2.f, 1.5, 1.5, 2.5, */    2.5, 1.5, 1.5, 2.f, 2.f, 4.f,
                            /*0.5, 0.f, */    0.f, 0.5
    };
    // clang-format on

    return costs[current.keyRef.row * 6 + current.keyRef.column];
}
} // namespace finger_tracking::penalties

#endif // PENALIZE_BASE_PENALTY_HPP
