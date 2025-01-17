#include "penalty_calculator.hpp"

#include "layout_generator/penalties/ngraphs.hpp"

#include <numeric>

namespace finger_tracking {
namespace {

template <size_t N, typename Fn>
auto processOneNGraph(Fn penalty) {
    return [penalty](float innerAccumulator, std::pair<NGraph<N>, size_t> const& pair) {
        return innerAccumulator + std::apply(penalty, pair.first.keys) * pair.second;
    };
}

template <size_t N, typename Fn>
float processAllNGraphsForPenalty(absl::flat_hash_map<NGraph<N>, size_t> const& ngraphs,
                                  Fn&&                                          penalty) {
    return std::reduce(ngraphs.begin(), ngraphs.end(), 0.f, processOneNGraph<N>(penalty));
}
} // namespace

float PenaltyCalculator::computePenalties(NGraphSet const& ngraphSet) const {
    float digraphPenalty = std::reduce(
        m_digraphPenalties.begin(), m_digraphPenalties.end(), 0.f,
        [ngraphSet](float accu, std::pair<digraph_fn, float> penalty) {
            return accu
                 + processAllNGraphsForPenalty(ngraphSet.digraphOccurrences, penalty.first)
                       * penalty.second;
        });

    float trigraphPenalty = std::reduce(
        m_trigraphPenalties.begin(), m_trigraphPenalties.end(), 0.f,
        [ngraphSet](float accu, std::pair<trigraph_fn, float> penalty) {
            return accu
                 + processAllNGraphsForPenalty(ngraphSet.trigraphOccurrences, penalty.first)
                       * penalty.second;
        });

    float quartadPenalty = std::reduce(
        m_quartadPenalties.begin(), m_quartadPenalties.end(), 0.f,
        [ngraphSet](float accu, std::pair<quartad_fn, float> penalty) {
            return accu
                 + processAllNGraphsForPenalty(ngraphSet.quartadOccurrences, penalty.first)
                       * penalty.second;
        });

    return digraphPenalty + trigraphPenalty + quartadPenalty;
}
} // namespace finger_tracking
