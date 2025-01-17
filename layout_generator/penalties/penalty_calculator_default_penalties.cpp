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
        {penalizeBaseCost, 1.f},
        {penalizeDistanceToHomeRow, 4.f},
        {penalizeSameFingerTwice, 4.f},
        {penalizeLongJumpSameHand, 1.f},
        {penalizeLongJumpSameFinger, 10.f},
        {penalizeLongJumpConsecutiveFingers, 5.f},
        {penalizeFingerTwist, 6.f},
        {penalizeRollOut, 0.125f},
        {penalizeRollIn, -0.125f},
    };

    result.m_trigraphPenalties = {
        {penalizeExteriorRollReversal, 20.f},
        {penalizeThreeFingerTwist, 10.f},
        {penalizeLongJumpSandwich, 3.f},
    };

    result.m_quartadPenalties = {
        {penalizeFourTimeSameHand, 0.5f},
        {penalizeFourAlternatingHands, 0.5f},
    };
    return result;
}
} // namespace finger_tracking
