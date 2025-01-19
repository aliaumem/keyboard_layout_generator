#include "layout_generator/key_press_calculator/key_press_calculator.hpp"
#include "layout_generator/azerty_voyager_layout.hpp"

#include "layout_generator/penalties/penalty_calculator.hpp"
#include "penalties/ngraphs.hpp"

#include "layout_generator/build_corpus.hpp"

#include <fmt/format.h>
#include <fmt/chrono.h>
#include <iostream>

using namespace finger_tracking;

float penaltiesForKeys(std::size_t size, std::vector<KeyPress> const& totalKeys) {
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

float computeLayoutPenalty(std::string_view                          corpus,
                           std::vector<static_vector<Key, 4>> const& shortcuts,
                           TargetKeyboardLayout const&               layout) {
    Simulator sim{layout};

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

int main(int argc, char* argv[]) {

    // - Compute the key sequence for the quartad
    // - Compute registered penalties
    // - Fill in penalties
    // - Mutate layouts
    // - Do the annealing

    try {
        std::vector<std::filesystem::path> files;
        for (int i = 1; i < argc; ++i) {
            files.emplace_back(argv[i]);
        }
        std::string corpus = buildCorpus(files);

        std::vector<static_vector<Key, 4>> shortcuts{
            {Key{"LCtl"}, Key{"Tab"}},
            {Key{"LCtl"}, Key{"LSft"}, Key{"Tab"}},
            {Key{"LAlt"}, Key{"Tab"}},
            {Key{"LAlt"}, Key{"LSft"}, Key{"Tab"}},
            {Key{"LCtl"}, Key{"X"}},
            {Key{"LCtl"}, Key{"C"}},
            {Key{"LCtl"}, Key{"V"}},
            {Key{"LCtl"}, Key{"Z"}},
            {Key{"LCtl"}, Key{"LSft"}, Key{"Z"}},
            // Directional arrows
            {Key{"LArr"}},
            {Key{"RArr"}},
            {Key{"UArr"}},
            {Key{"DArr"}},
            // Moving through code
            {Key{"LCtl"}, Key{"LArr"}},
            {Key{"LCtl"}, Key{"RArr"}},
            {Key{"LSft"}, Key{"LArr"}},
            {Key{"LSft"}, Key{"RArr"}},
            {Key{"LSft"}, Key{"UArr"}},
            {Key{"LSft"}, Key{"DArr"}},
        };

        auto layout = azertyVoyagerLayout();
        computeLayoutPenalty(corpus, shortcuts, layout);

    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
