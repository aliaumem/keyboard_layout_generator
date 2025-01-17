#ifndef PENALIZE_QUARTADS_HPP
#define PENALIZE_QUARTADS_HPP

#include "layout_generator/penalties/ngraphs.hpp"

namespace finger_tracking {
inline float penalizeFourTimeSameHand(KeyPress const& current, KeyPress const& previous1,
                                      KeyPress const& previous2, KeyPress const& previous3) {
    return current.side() == previous1.side() && previous1.side() == previous2.side()
                && previous2.side() == previous3.side()
             ? 0.5f
             : 0;
}

inline float penalizeFourAlternatingHands(KeyPress const& current, KeyPress const& previous1,
                                          KeyPress const& previous2, KeyPress const& previous3) {
    return current.side() != previous1.side() && previous1.side() != previous2.side()
                && previous2.side() != previous3.side()
             ? 0.5
             : 0;
}
} // namespace finger_tracking
#endif // PENALIZE_QUARTADS_HPP
