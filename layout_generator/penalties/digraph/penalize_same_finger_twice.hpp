#ifndef PENALIZE_SAME_FINGER_TWICE_HPP
#define PENALIZE_SAME_FINGER_TWICE_HPP

#include "layout_generator/penalties/ngraphs.hpp"

namespace finger_tracking::penalties {
inline float penalizeSameFingerTwice(KeyPress const& current, KeyPress const& previous) {
    bool isSameFinger = current.fingerDesc() == previous.fingerDesc();
    return current.keyRef != previous.keyRef && isSameFinger ? 4.f : 0.f;
}

inline float penalizeLongJumpSameHand(KeyPress const& current, KeyPress const& previous) {
    bool isSameHand   = current.side() == previous.side();
    bool crossHomeRow = (current.row() > Row::Home && previous.row() < Row::Home)
                     || (current.row() < Row::Home && previous.row() > Row::Home);

    return isSameHand && crossHomeRow ? 1.f : 0.f;
}

inline float penalizeLongJumpSameFinger(KeyPress const& current, KeyPress const& previous) {
    bool isSameFinger = current.fingerDesc() == previous.fingerDesc();
    bool crossHomeRow = (current.row() > Row::Home && previous.row() < Row::Home)
                     || (current.row() < Row::Home && previous.row() > Row::Home);

    return isSameFinger && crossHomeRow ? 1 : 0.f;
}

inline float penalizeLongJumpConsecutiveFingers(KeyPress const& current, KeyPress const& previous) {
    bool areConsecutiveFinger
        = current.side() == previous.side() && std::abs(current.finger - previous.finger) == 1;

    bool crossHomeRow = (current.row() > Row::Home && previous.row() < Row::Home)
                     || (current.row() < Row::Home && previous.row() > Row::Home);

    return areConsecutiveFinger && crossHomeRow ? 1.f : 0.f;
}

inline float penalizeFingerTwist(KeyPress const& current, KeyPress const& previous) {
    bool isSameHand         = current.side() == previous.side();
    bool ringDownAfterPinky = current.finger == Finger::Ring && previous.finger == Finger::Pinky
                           && current.row() < previous.row();
    bool pinkyUpAfterRing = current.finger == Finger::Pinky && previous.finger == Finger::Ring
                         && current.row() > previous.row();

    return isSameHand && (ringDownAfterPinky || pinkyUpAfterRing)
             ? std::abs(current.row() - previous.row())
             : 0.f;
}

inline float penalizeRollOut(KeyPress const& current, KeyPress const& previous) {
    bool isSameHand = current.side() == previous.side();
    bool isRollOut  = current.finger != Finger::Thumb && previous.finger != Finger::Thumb
                  && current.finger > previous.finger;

    return isSameHand && isRollOut ? 1 : 0.f;
}

inline float penalizeRollIn(KeyPress const& current, KeyPress const& previous) {
    bool isSameHand = current.side() == previous.side();
    bool isRollIn   = current.finger < previous.finger;

    return isSameHand && isRollIn ? 1 : 0.f;
}
} // namespace finger_tracking::penalties
#endif // PENALIZE_SAME_FINGER_TWICE_HPP
