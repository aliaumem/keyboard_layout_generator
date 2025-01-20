#ifndef ANNEALING_HPP
#define ANNEALING_HPP

#include <cmath>
#include <random>

namespace finger_tracking {

class Annealing {
public:
    static constexpr double T0 = 1.5f;
    static constexpr double K  = 10.0f;
    static constexpr double P0 = 1.0f;
    static constexpr size_t N  = 15000;
    static constexpr double KN = K / static_cast<double>(N);

    Annealing()
        : m_numberDistribution{0, 1} {}

    template <typename Generator>
    auto accept_transition(double de, size_t i, Generator& generator) -> bool {
        // For positive dE, accept if r < p_dE where r ~ Uniform(0, 1)
        if (de < 0.0) {
            return true;
        } else {
            auto p_de = cutoff_p(de, i);
            auto r    = m_numberDistribution(generator);
            return r < p_de;
        }
    }

private:
    std::uniform_real_distribution<double> m_numberDistribution;

    // T(i) = T0 exp(-ik/N)
    auto temperature(size_t i) -> double { return T0 * std::exp(-static_cast<double>(i) * KN); }

    // p(dE, i) = p0 exp(-dE/T(i))
    auto cutoff_p(double de, size_t i) -> double {
        auto t = temperature(i);
        return P0 * std::exp(-de / t);
    }
};
} // namespace finger_tracking

#endif // ANNEALING_HPP
