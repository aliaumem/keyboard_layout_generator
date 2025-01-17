#include "penalty_calculator.hpp"

#include "layout_generator/penalties/ngraphs.hpp"
#include "penalize_base_penalty.hpp"
#include "penalize_digraphs.hpp"
#include "layout_generator/penalties/penalize_trigraph.hpp"
#include "layout_generator/penalties/penalize_quartads.hpp"

namespace finger_tracking {
using namespace penalties;

PenaltyCalculator PenaltyCalculator::defaultPenalties() {
    PenaltyCalculator result;
    result.m_digraphPenalties = {
        {penalizeBaseCost, 1},
        {penalizeSameFingerTwice, 32},
        {penalizeLongJumpSameHand, 8},
        {penalizeLongJumpSameFinger, 80},
        {penalizeLongJumpConsecutiveFingers, 40},
        {penalizeFingerTwist, 48},
        {penalizeRollOut, 1},
        {penalizeRollIn, -1},
    };

    result.m_trigraphPenalties = {
        {penalizeExteriorRollReversal, 160},
        {penalizeThreeFingerTwist, 80},
        {penalizeLongJumpSandwich, 24},
    };

    result.m_quartadPenalties = {
        {penalizeFourTimeSameHand, 4},
        {penalizeFourAlternatingHands, 4},
    };
    return result;
}
} // namespace finger_tracking
