#ifndef PENALTY_CALCULATOR_HPP
#define PENALTY_CALCULATOR_HPP

#include <vector>

namespace finger_tracking {
struct KeyPress;
struct NGraphSet;

class PenaltyCalculator {
public:
    static PenaltyCalculator defaultPenalties();
    std::int64_t             computePenalties(NGraphSet const& ngraphSet) const;

private:
    using digraph_fn  = int64_t (*)(KeyPress const&, KeyPress const&);
    using trigraph_fn = int64_t (*)(KeyPress const&, KeyPress const&, KeyPress const&);
    using quartad_fn
        = int64_t (*)(KeyPress const&, KeyPress const&, KeyPress const&, KeyPress const&);

    std::vector<std::pair<digraph_fn, int64_t>>  m_digraphPenalties;
    std::vector<std::pair<trigraph_fn, int64_t>> m_trigraphPenalties;
    std::vector<std::pair<quartad_fn, int64_t>>  m_quartadPenalties;
};
} // namespace finger_tracking

#endif // PENALTY_CALCULATOR_HPP
