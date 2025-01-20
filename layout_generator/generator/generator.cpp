#include "generator.hpp"

#include "layout_generator/mutation/layout_mutator.hpp"

#include <fmt/format.h>
#include <fmt/chrono.h>

#include "range/v3/view/iota.hpp"
#include <random>

namespace finger_tracking {
auto Generator::run(TargetKeyboardLayout const& initialLayout) -> TargetKeyboardLayout {
    float currentPenalty = computeLayoutPenalty(initialLayout);

    std::vector<TargetKeyboardLayout> bestLayouts;

    TargetKeyboardLayout acceptedLayout  = initialLayout;
    float                acceptedPenalty = currentPenalty;

    std::mt19937_64 generator{std::random_device{}()};

    for (auto i : ranges::views::ints(0, 15)) {
        auto                                         currentLayout{acceptedLayout};
        LayoutMutator                                mutator{currentLayout};
        std::uniform_int_distribution<std::uint16_t> numSwapsDistribution{1, 4};
        mutator.performNSwapsOrCopies(numSwapsDistribution(generator), generator);
        currentPenalty = computeLayoutPenalty(currentLayout);
        /*if (currentPenalty - acceptedPenalty < 0)*/ {
            acceptedLayout  = currentLayout;
            acceptedPenalty = currentPenalty;
            // bestLayouts.emplace_back(acceptedLayout);
        }
    }

    return acceptedLayout;
}

float Generator::computeLayoutPenalty(TargetKeyboardLayout const& layout) {
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
} // namespace finger_tracking
