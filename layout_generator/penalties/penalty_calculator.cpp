#include "penalty_calculator.hpp"

#include "layout_generator/penalties/ngraphs.hpp"

#include <numeric>
#include <execution>

namespace finger_tracking {
namespace {

template <size_t N, typename Fn>
auto processOneNGraph(Fn penalty) {
    return [penalty](NGraph<N> const& ngraph) -> int64_t {
        return std::apply(penalty, ngraph.keys);
    };
}

template <size_t N, typename Fn>
int64_t processAllNGraphsForPenalty(std::vector<NGraph<N>> const& ngraphs, Fn&& penalty) {
    return std::transform_reduce(ngraphs.begin(), ngraphs.end(), 0ll, std::plus{}, processOneNGraph<N>(penalty));
}
} // namespace

std::int64_t PenaltyCalculator::computePenalties(NGraphSet const& ngraphSet) const {
    auto policy = std::execution::par_unseq;

    int64_t     digraphPenalty;
    std::thread digraphThread{[&]() {
        digraphPenalty = std::transform_reduce(
            policy, m_digraphPenalties.begin(), m_digraphPenalties.end(), std::int64_t{0},
            std::plus<int64_t>{},
            [ngraphSet](std::pair<digraph_fn, std::int64_t> const& penalty) -> int64_t {
                return processAllNGraphsForPenalty(ngraphSet.digraphOccurrences, penalty.first)
                     * penalty.second;
            });
    }};

    int64_t     trigraphPenalty;
    std::thread trigraphThread{[&]() {
        trigraphPenalty = std::transform_reduce(
            policy, m_trigraphPenalties.begin(), m_trigraphPenalties.end(), 0ll,
            std::plus<int64_t>{}, [ngraphSet](std::pair<trigraph_fn, int64_t> const& penalty) {
                return processAllNGraphsForPenalty(ngraphSet.trigraphOccurrences, penalty.first)
                     * penalty.second;
            });
    }};

    int64_t     quartadPenalty;
    std::thread quartadThread{[&]() {
        quartadPenalty = std::transform_reduce(
            policy, m_quartadPenalties.begin(), m_quartadPenalties.end(), 0ll, std::plus<int64_t>{},
            [ngraphSet](std::pair<quartad_fn, int64_t> const& penalty) {
                return processAllNGraphsForPenalty(ngraphSet.quartadOccurrences, penalty.first)
                     * penalty.second;
            });
    }};

    digraphThread.join();
    trigraphThread.join();
    quartadThread.join();

    return (digraphPenalty + trigraphPenalty + quartadPenalty) / 8;
    // return digraphPenalty / 8;
}
} // namespace finger_tracking
