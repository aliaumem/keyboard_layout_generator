#include "penalty_calculator.hpp"

#include "layout_generator/penalties/ngraphs.hpp"
#include "layout_generator/penalties/digraph/penalize_distance_to_home_row.hpp"
#include "layout_generator/penalties/digraph/penalize_base_penalty.hpp"
#include "layout_generator/penalties/digraph/penalize_same_finger_twice.hpp"
#include "layout_generator/penalties/penalize_trigraph.hpp"
#include "layout_generator/penalties/penalize_quartads.hpp"

namespace finger_tracking {
using namespace penalties;

PenaltyCalculator PenaltyCalculator::defaultPenalties() {
    PenaltyCalculator result;
    result.m_digraphPenalties = {
        penalizeBaseCost,
        penalizeDistanceToHomeRow,
        penalizeSameFingerTwice,
        penalizeLongJumpSameHand,
        penalizeLongJumpSameFinger,
        penalizeLongJumpConsecutiveFingers,
        penalizeFingerTwist,
        penalizeRollOut,
        penalizeRollIn,
    };

    result.m_trigraphPenalties = {
        penalizeExteriorRollReversal,
        penalizeThreeFingerTwist,
        penalizeLongJumpSandwich,
    };

    result.m_quartadPenalties = {
        penalizeFourTimeSameHand,
        penalizeFourAlternatingHands,
    };
    return result;
}
} // namespace finger_tracking
