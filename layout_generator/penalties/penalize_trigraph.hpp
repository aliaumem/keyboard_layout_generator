#ifndef PENALIZE_TRIGRAPH_HPP
#define PENALIZE_TRIGRAPH_HPP

#include "layout_generator/penalties/ngraphs.hpp"

namespace finger_tracking {
inline float penalizeExteriorRollReversal(KeyPress const& current, KeyPress const& previous1,
                                          KeyPress const& previous2) {
    using enum Finger;

    bool isSameHand = current.side() == previous1.side() && current.side() == previous2.side();

    auto seq = std::tie(current.finger, previous1.finger, previous2.finger);
    bool isReverseExteriorRoll
        = seq == std::tuple{Middle, Pinky, Ring} || seq == std::tuple{Ring, Pinky, Middle};

    return isSameHand && isReverseExteriorRoll ? 20 : 0;
}

inline float penalizeThreeFingerTwist(KeyPress const& current, KeyPress const& previous1,
                                      KeyPress const& previous2) {
    using enum Finger;

    bool isSameHand = current.side() == previous1.side() && current.side() == previous2.side();

    bool isMonotonicRow = (current.row() < previous1.row() && previous1.row() < previous2.row())
                       || (current.row() > previous1.row() && previous1.row() > previous2.row());

    bool isRollOut = current.finger > previous1.finger && previous1.finger > previous2.finger;
    bool isRollIn  = current.finger < previous1.finger && previous1.finger < previous2.finger;

    return isSameHand && isMonotonicRow && (isRollOut || isRollIn) ? 10 : 0;
}

inline float penalizeLongJumpSandwich(KeyPress const& current, KeyPress const& /*previous1*/,
                                      KeyPress const& previous2) {
    bool isSameFinger = current.fingerDesc() == previous2.fingerDesc();
    bool crossHomeRow = (current.row() > Row::Home && previous2.row() < Row::Home)
                     || (current.row() < Row::Home && previous2.row() > Row::Home);

    return isSameFinger && crossHomeRow ? 3 : 0;
}
} // namespace finger_tracking

#endif // PENALIZE_TRIGRAPH_HPP
