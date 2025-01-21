#include "penalty_calculator.hpp"

#include "layout_generator/penalties/ngraphs.hpp"

#include <numeric>
#include <execution>

namespace finger_tracking {
namespace {

template <size_t N, typename Fn>
auto processOneNGraph(Fn penalty) {
    return
        [penalty](NGraph<N> const& ngraph) -> int64_t { return std::apply(penalty, ngraph.keys); };
}

template <size_t N, typename Fn>
int64_t processAllNGraphsForPenalty(std::vector<NGraph<N>> const& ngraphs, Fn&& penalty) {
    return std::transform_reduce(
        ngraphs.begin(), ngraphs.end(), 0ll, std::plus{}, processOneNGraph<N>(penalty));
}
} // namespace

std::int64_t PenaltyCalculator::computePenalties(NGraphSet const& ngraphSet) const {

    int64_t digraphPenalty
        = processAllNGraphsForPenalty(ngraphSet.digraphOccurrences, m_digraphPenalties);

    int64_t trigraphPenalty
        = processAllNGraphsForPenalty(ngraphSet.trigraphOccurrences, m_trigraphPenalties);

    int64_t quartadPenalty
        = processAllNGraphsForPenalty(ngraphSet.quartadOccurrences, m_quartadPenalties);

    return (digraphPenalty + trigraphPenalty + quartadPenalty) / 8;
}
} // namespace finger_tracking
