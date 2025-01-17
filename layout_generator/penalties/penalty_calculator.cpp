#include "penalty_calculator.hpp"

#include "layout_generator/penalties/ngraphs.hpp"

#include <numeric>

namespace finger_tracking {
namespace {

template <size_t N, typename Fn>
auto processOneNGraph(Fn penalty) {
    return [penalty](int64_t innerAccumulator, std::pair<NGraph<N>, size_t> const& pair) {
        return innerAccumulator + std::apply(penalty, pair.first.keys) * pair.second;
    };
}

template <size_t N, typename Fn>
int64_t processAllNGraphsForPenalty(absl::flat_hash_map<NGraph<N>, size_t> const& ngraphs,
                                    Fn&&                                          penalty) {
    return std::reduce(ngraphs.begin(), ngraphs.end(), 0ll, processOneNGraph<N>(penalty));
}
} // namespace

std::int64_t PenaltyCalculator::computePenalties(NGraphSet const& ngraphSet) const {
    int64_t digraphPenalty = std::reduce(
        m_digraphPenalties.begin(), m_digraphPenalties.end(), 0ll,
        [ngraphSet](int64_t accu, std::pair<digraph_fn, int64_t> penalty) {
            return accu
                 + processAllNGraphsForPenalty(ngraphSet.digraphOccurrences, penalty.first)
                       * penalty.second;
        });

    int64_t trigraphPenalty = std::reduce(
        m_trigraphPenalties.begin(), m_trigraphPenalties.end(), 0ll,
        [ngraphSet](int64_t accu, std::pair<trigraph_fn, int64_t> penalty) {
            return accu
                 + processAllNGraphsForPenalty(ngraphSet.trigraphOccurrences, penalty.first)
                       * penalty.second;
        });

    int64_t quartadPenalty = std::reduce(
        m_quartadPenalties.begin(), m_quartadPenalties.end(), 0ll,
        [ngraphSet](int64_t accu, std::pair<quartad_fn, int64_t> penalty) {
            return accu
                 + processAllNGraphsForPenalty(ngraphSet.quartadOccurrences, penalty.first)
                       * penalty.second;
        });

    return (digraphPenalty + trigraphPenalty + quartadPenalty) / 8;
}
} // namespace finger_tracking
