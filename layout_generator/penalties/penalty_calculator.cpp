#include "penalty_calculator.hpp"

#include "layout_generator/row_col.hpp"

#include <numeric>

namespace finger_tracking {
namespace {
float penalizeDistanceToHomeRow(Quartad const& sequence) {
    auto keyRef = *sequence.begin();
    return 4.f * static_cast<float>(std::abs(keyRef.keyRef.row - static_cast<int>(Row::Home)));
}

float penalizeSameFingerTwice(Quartad const& sequence) {
    auto current  = *sequence.begin();
    auto previous = *++sequence.begin();

    bool isSameFinger = current.fingerDesc() == previous.fingerDesc();

    return current.keyRef != previous.keyRef && isSameFinger ? 4.f : 0.f;
}
} // namespace

PenaltyCalculator::PenaltyCalculator()
    : m_penalties{penalizeDistanceToHomeRow, penalizeSameFingerTwice} {}

float PenaltyCalculator::computePenalties(std::vector<Quartad> const& quartads) const {
    return std::accumulate(
        quartads.begin(), quartads.end(), 0.f, [this](float acc, Quartad const& quartad) {
            return std::accumulate(m_penalties.begin(), m_penalties.end(), 0.f,
                                   [&quartad](float innerAcc, auto const& penalty) {
                                       return innerAcc + penalty(quartad);
                                   })
                 + acc;
        });
}
} // namespace finger_tracking
