#ifndef PENALTY_CALCULATOR_HPP
#define PENALTY_CALCULATOR_HPP

#include <vector>
#include <cstdint>

namespace finger_tracking {
struct KeyPress;
struct NGraphSet;

class PenaltyCalculator {
public:
    static PenaltyCalculator defaultPenalties();
    std::int64_t             computePenalties(NGraphSet const& ngraphSet) const;

private:
    using digraph_fn  = int64_t (*)(KeyPress, KeyPress);
    using trigraph_fn = int64_t (*)(KeyPress, KeyPress, KeyPress);
    using quartad_fn  = int64_t (*)(KeyPress, KeyPress, KeyPress, KeyPress);

    digraph_fn  m_digraphPenalties;
    trigraph_fn m_trigraphPenalties;
    quartad_fn  m_quartadPenalties;
};
} // namespace finger_tracking

#endif // PENALTY_CALCULATOR_HPP
