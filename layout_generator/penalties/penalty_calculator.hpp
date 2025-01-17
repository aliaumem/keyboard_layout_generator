#ifndef PENALTY_CALCULATOR_HPP
#define PENALTY_CALCULATOR_HPP

#include <vector>

namespace finger_tracking {
template <size_t N>
struct NGraph;
using Quartad  = NGraph<4>;
using Trigraph = NGraph<3>;
using Digraph  = NGraph<2>;
struct NGraphSet;

class PenaltyCalculator {
public:
    static PenaltyCalculator defaultPenalties();
    float                    computePenalties(NGraphSet const& ngraphSet) const;

private:
    std::vector<float (*)(Digraph const&)>  m_digraphPenalties;
    std::vector<float (*)(Trigraph const&)> m_trigraphPenalties;
    std::vector<float (*)(Quartad const&)>  m_quartadPenalties;
};
} // namespace finger_tracking

#endif // PENALTY_CALCULATOR_HPP
