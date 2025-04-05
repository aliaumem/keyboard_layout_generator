#ifndef PENALIZE_BASE_PENALTY_HPP
#define PENALIZE_BASE_PENALTY_HPP

#include "layout_generator/penalties/ngraphs.hpp"

namespace finger_tracking::penalties {
inline int64_t penalizeBaseCost(KeyPress const& current, KeyPress const& /*previous*/) {
    // clang-format off
    std::array<float, 26> costs {
        /*4.f, 5.f, 4.f, 3.f, 3.f, 4.f, */    5.f, 4.f, 3.f, 4.f, 5.f, 3.f,
        /*3.f, 3.f, 1.f, 1.f, 1.5, 3.f, */    2.f, 1.5, 1.f, 1.f, 3.f, 4.f,
        /*2.f, 0.5, 0.5, 0.f, 0.f, 1.5, */    1.5, 0.f, 0.f, 0.5, 0.5, 2.5,
        /*4.f, 2.f, 2.f, 1.5, 1.5, 2.5, */    3.5, 1.5, 1.5, 2.f, 2.f, 3.f,
                            /*0.5, 0.f, */    0.f, 0.5
    };
    // clang-format on

    return costs[static_cast<int64_t>(current.keyRef.row) * 6
                 + static_cast<int64_t>(current.keyRef.column)]
         * 8;
}
} // namespace finger_tracking::penalties

#endif // PENALIZE_BASE_PENALTY_HPP
