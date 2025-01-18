#include "layout_generator/simulator/simulator.hpp"
#include "layout_generator/azerty_voyager_layout.hpp"

#include "layout_generator/penalties/penalty_calculator.hpp"
#include "penalties/ngraphs.hpp"

#include "layout_generator/build_corpus.hpp"

#include <format>
#include <iostream>
#include <fstream>

using namespace finger_tracking;

int main(int argc, char* argv[]) {

    // - Compute the key sequence for the quartad
    // - Compute registered penalties
    // - Fill in penalties
    // - Do the annealing

    try {

        std::vector<std::filesystem::path> files;
        for (int i = 1; i < argc; ++i) {
            files.emplace_back(argv[i]);
        }
        std::string book = buildCorpus(files);

        auto      layout = azertyVoyagerLayout();
        Simulator sim{layout};
        auto      startSim  = std::chrono::high_resolution_clock::now();
        auto      totalKeys = sim.simulate(book);
        auto      endSim    = std::chrono::high_resolution_clock::now();
        std::cout << totalKeys.size() << " out of " << totalKeys.capacity() << "\t"
                  << std::chrono::duration_cast<std::chrono::milliseconds>(endSim - startSim)
                  << std::endl;

        auto startNGraph = std::chrono::high_resolution_clock::now();
        auto quartadSet  = NGraphSet::computeSetFromKeyPresses(totalKeys);
        auto endNGraph   = std::chrono::high_resolution_clock::now();
        std::cout << "ngraphs: " << quartadSet.size() << "\t"
                  << std::chrono::duration_cast<std::chrono::milliseconds>(endNGraph - startNGraph)
                  << std::endl;

        PenaltyCalculator penaltyCalculator = PenaltyCalculator::defaultPenalties();
        auto              startPenalties    = std::chrono::high_resolution_clock::now();
        int64_t           penalty           = penaltyCalculator.computePenalties(quartadSet);
        auto              endPenalties      = std::chrono::high_resolution_clock::now();
        std::cout << std::format(
            "penalty : {}, normalized: {} ({})", penalty,
            static_cast<float>(penalty) / static_cast<float>(book.size()),
            std::chrono::duration_cast<std::chrono::milliseconds>(endPenalties - startPenalties))
                  << std::endl;

    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
