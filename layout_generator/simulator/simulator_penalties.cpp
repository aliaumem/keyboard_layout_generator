#include "simulator.hpp"

namespace finger_tracking {
static float computePenalty(KeyLayoutSequence const& sequence) {
    auto keyRef = sequence.front();

    return 4.f * static_cast<float>(std::abs(keyRef.row - static_cast<int>(Row::Home)));
}

Simulator::Simulator(KeyboardLayout<52> const& layout)
    : m_layout{layout} {
    m_penalties.emplace_back(computePenalty);
}

} // namespace finger_tracking
