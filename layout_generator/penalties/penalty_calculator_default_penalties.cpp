#include "penalty_calculator.hpp"

#include "layout_generator/penalties/ngraphs.hpp"
#include "layout_generator/penalties/digraph/penalize_distance_to_home_row.hpp"
#include "layout_generator/penalties/digraph/penalize_same_finger_twice.hpp"

namespace finger_tracking {
using namespace penalties;

PenaltyCalculator PenaltyCalculator::defaultPenalties() {
    PenaltyCalculator result;
    result.m_digraphPenalties = {penalizeDistanceToHomeRow, penalizeSameFingerTwice};
    return result;
}
} // namespace finger_tracking
