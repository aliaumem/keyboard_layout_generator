#ifndef PENALTY_CALCULATOR_HPP
#define PENALTY_CALCULATOR_HPP

#include <vector>

namespace finger_tracking {
struct KeyPress;
struct NGraphSet;

class PenaltyCalculator {
public:
    static PenaltyCalculator defaultPenalties();
    float                    computePenalties(NGraphSet const& ngraphSet) const;

private:
    using digraph_fn  = float (*)(KeyPress const&, KeyPress const&);
    using trigraph_fn = float (*)(KeyPress const&, KeyPress const&, KeyPress const&);
    using quartad_fn
        = float (*)(KeyPress const&, KeyPress const&, KeyPress const&, KeyPress const&);

    std::vector<std::pair<digraph_fn, float>>  m_digraphPenalties;
    std::vector<std::pair<trigraph_fn, float>> m_trigraphPenalties;
    std::vector<std::pair<quartad_fn, float>>  m_quartadPenalties;
};
} // namespace finger_tracking

#endif // PENALTY_CALCULATOR_HPP
