#ifndef PENALTY_CALCULATOR_HPP
#define PENALTY_CALCULATOR_HPP

#include "layout_generator/key_press.hpp"

#include <array>
#include <vector>

namespace finger_tracking {
struct Quartad {
    std::array<KeyPress, 4> keys;
    auto                    begin() const { return keys.begin(); }
    auto                    end() const { return keys.end(); }
};

class PenaltyCalculator {
public:
    PenaltyCalculator();
    float computePenalties(std::vector<Quartad> const& quartads) const;

private:
    std::vector<float (*)(Quartad const&)> m_penalties;
};

// static_assert(sizeof(Quartad) == 1);

} // namespace finger_tracking

#endif // PENALTY_CALCULATOR_HPP
