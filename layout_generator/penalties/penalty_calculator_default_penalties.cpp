#include "penalty_calculator.hpp"

#include "layout_generator/penalties/ngraphs.hpp"
#include "penalize_base_penalty.hpp"
#include "penalize_digraphs.hpp"
#include "layout_generator/penalties/penalize_trigraph.hpp"
#include "layout_generator/penalties/penalize_quartads.hpp"

namespace finger_tracking {
using namespace penalties;

namespace {
int64_t processAllDigraphPenalties(KeyPress current, KeyPress previous) {
    return penalizeBaseCost(current, previous) * 1 + penalizeSameFingerTwice(current, previous) * 32
         + penalizeLongJumpSameHand(current, previous) * 8
         + penalizeLongJumpSameFinger(current, previous) * 80
         + penalizeLongJumpConsecutiveFingers(current, previous) * 40
         + penalizeFingerTwist(current, previous) * 48 + penalizeRollOut(current, previous) * 1
         + penalizeRollIn(current, previous) * -1 + penalizeLayerChange(current, previous) * 40;
}

int64_t processAllTrigraphPenalties(KeyPress current, KeyPress previous1, KeyPress previous2) {
    return penalizeExteriorRollReversal(current, previous1, previous2) * 160
         + penalizeThreeFingerTwist(current, previous1, previous2) * 80
         + penalizeLongJumpSandwich(current, previous1, previous2) * 24;
}
int64_t processAllQuartadPenalties(KeyPress current, KeyPress previous1, KeyPress previous2,
                                   KeyPress previous3) {
    return penalizeFourTimeSameHand(current, previous1, previous2, previous3) * 4
         + penalizeFourAlternatingHands(current, previous1, previous2, previous3) * 4;
}
} // namespace

PenaltyCalculator PenaltyCalculator::defaultPenalties() {
    PenaltyCalculator result;

    result.m_digraphPenalties = processAllDigraphPenalties;

    result.m_trigraphPenalties = processAllTrigraphPenalties;

    result.m_quartadPenalties = processAllQuartadPenalties;
    return result;
}
} // namespace finger_tracking
