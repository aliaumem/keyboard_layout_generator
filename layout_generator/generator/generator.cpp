#include "generator.hpp"

#include <fmt/format.h>
#include <fmt/chrono.h>

namespace finger_tracking {
float Generator::penaltiesForKeys(std::size_t size, std::vector<KeyPress> const& totalKeys) {
    auto startNGraph = std::chrono::high_resolution_clock::now();
    auto ngraphSet   = NGraphSet::computeSetFromKeyPresses(totalKeys);
    auto endNGraph   = std::chrono::high_resolution_clock::now();
    std::cout << "ngraphs: " << ngraphSet.size() << "\t"
              << std::chrono::duration_cast<std::chrono::milliseconds>(endNGraph - startNGraph)
              << std::endl;

    PenaltyCalculator penaltyCalculator = PenaltyCalculator::defaultPenalties();
    auto              startPenalties    = std::chrono::high_resolution_clock::now();
    int64_t           penalty           = penaltyCalculator.computePenalties(ngraphSet);
    auto              endPenalties      = std::chrono::high_resolution_clock::now();
    float             normalizedPenalty = static_cast<float>(penalty) / static_cast<float>(size);
    std::cout << fmt::format(
        "penalty : {}, normalized: {} ({})", penalty, normalizedPenalty,
        std::chrono::duration_cast<std::chrono::milliseconds>(endPenalties - startPenalties))
              << std::endl;

    return normalizedPenalty;
}

float Generator::computeLayoutPenalty(std::string_view                          corpus,
                                      std::vector<static_vector<Key, 4>> const& shortcuts,
                                      TargetKeyboardLayout const&               layout) {
    KeyPressCalculator sim{layout};

    auto startSim  = std::chrono::high_resolution_clock::now();
    auto totalKeys = sim.simulate(corpus);
    auto endSim    = std::chrono::high_resolution_clock::now();

    std::cout << totalKeys.size() << " out of " << totalKeys.capacity() << "\t"
              << std::chrono::duration_cast<std::chrono::milliseconds>(endSim - startSim)
              << std::endl;

    auto shortcutKeys = sim.simulateShortcuts(shortcuts);

    auto p1 = penaltiesForKeys(corpus.size(), totalKeys);
    auto p2 = penaltiesForKeys(shortcuts.size(), shortcutKeys);

    return std::midpoint(p1, p2);
}
} // namespace finger_tracking
