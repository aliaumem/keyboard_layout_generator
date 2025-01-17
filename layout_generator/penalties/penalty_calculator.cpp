#include "penalty_calculator.hpp"

#include "layout_generator/penalties/ngraphs.hpp"
#include "layout_generator/row_col.hpp"

#include <numeric>

namespace finger_tracking {
namespace {
float penalizeDistanceToHomeRow(Digraph const& digraph) {
    auto keyRef = digraph.current();
    return 4.f * static_cast<float>(std::abs(keyRef.keyRef.row - static_cast<int>(Row::Home)));
}

float penalizeSameFingerTwice(Digraph const& digraph) {
    auto current  = digraph.current();
    auto previous = digraph.prev1();

    bool isSameFinger = current.fingerDesc() == previous.fingerDesc();

    return current.keyRef != previous.keyRef && isSameFinger ? 4.f : 0.f;
}
} // namespace

PenaltyCalculator::PenaltyCalculator()
    : m_digraphPenalties{penalizeDistanceToHomeRow, penalizeSameFingerTwice} {}

namespace {
template <size_t N>
using PenaltyFn = float (*)(NGraph<N> const&);

template <size_t N>
auto processOneNGraph(PenaltyFn<N> penalty) {
    return [penalty](float innerAccumulator, std::pair<NGraph<N>, size_t> const& pair) {
        return innerAccumulator + penalty(pair.first) * pair.second;
    };
}

template <size_t N>
float processAllNGraphsForPenalty(absl::flat_hash_map<NGraph<N>, size_t> const& ngraphs,
                                  PenaltyFn<N>                                  penalty) {
    return std::reduce(ngraphs.begin(), ngraphs.end(), 0.f, processOneNGraph(penalty));
}
} // namespace

float PenaltyCalculator::computePenalties(NGraphSet const& ngraphSet) const {
    float digraphPenalty = std::reduce(
        m_digraphPenalties.begin(), m_digraphPenalties.end(), 0.f,
        [ngraphSet](float accu, PenaltyFn<2> penalty) {
            return accu + processAllNGraphsForPenalty(ngraphSet.digraphOccurrences, penalty);
        });

    float trigraphPenalty = std::reduce(
        m_trigraphPenalties.begin(), m_trigraphPenalties.end(), 0.f,
        [ngraphSet](float accu, PenaltyFn<3> penalty) {
            return accu + processAllNGraphsForPenalty(ngraphSet.trigraphOccurrences, penalty);
        });

    float quartadPenalty = std::reduce(
        m_quartadPenalties.begin(), m_quartadPenalties.end(), 0.f,
        [ngraphSet](float accu, PenaltyFn<4> penalty) {
            return accu + processAllNGraphsForPenalty(ngraphSet.quartadOccurrences, penalty);
        });

    return digraphPenalty + trigraphPenalty + quartadPenalty;
}
} // namespace finger_tracking
