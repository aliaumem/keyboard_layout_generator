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

    std::vector<digraph_fn>  m_digraphPenalties;
    std::vector<trigraph_fn> m_trigraphPenalties;
    std::vector<quartad_fn>  m_quartadPenalties;
};
} // namespace finger_tracking

#endif // PENALTY_CALCULATOR_HPP
