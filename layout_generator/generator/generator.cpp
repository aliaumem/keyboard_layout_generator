#include "generator.hpp"

#include "annealing.hpp"
#include "worker.hpp"
#include "layout_generator/mutation/layout_mutator.hpp"

#include "layout_generator/keyboard_layout_print_helper.hpp"

#include <fmt/format.h>
#include <fmt/chrono.h>

#include "range/v3/view/iota.hpp"
#include "range/v3/view/drop.hpp"

#include <mutex>
#include <random>
#include <thread>
#include <range/v3/action/sort.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/zip.hpp>

namespace finger_tracking {
auto Generator::run(TargetKeyboardLayout const& initialLayout) const -> TargetKeyboardLayout {
    struct LayoutPenaltyPair {
        TargetKeyboardLayout layout;
        float                penalty;
    };

    constexpr size_t N = 20;
    std::vector      currentLayouts{
        N, LayoutPenaltyPair{initialLayout, computeLayoutPenalty(initialLayout)}};
    currentLayouts.reserve(N * 5);
    auto updatePenalty = [this](LayoutPenaltyPair& currentLayout) {
        currentLayout.penalty = computeLayoutPenalty(currentLayout.layout);
    };
    std::vector workers{N * 4, Worker<decltype(updatePenalty), LayoutPenaltyPair>{updatePenalty}};

    std::vector<std::thread> threads{};
    threads.reserve(N * 4);
    for (auto& worker : workers) {
        threads.emplace_back([&] { worker.run(); });
    }

    // std::mutex   layoutsMutex;
    std::mt19937_64 generator{std::random_device{}()};
    size_t          countSameLayout = 0;
    float           lastBestPenalty = currentLayouts.front().penalty;

    for (auto i : ranges::views::ints(0ull, 30000ull)) {
        // for (auto i : ranges::views::ints(0ull, 50ull)) {
        auto iterationStart = std::chrono::high_resolution_clock::now();
        for (auto const& [index, startingLayout] : currentLayouts | ranges::views::enumerate) {

            std::uniform_int_distribution<std::uint16_t> numSwapsDistribution{1, 5};
            for (int n = 0; n < std::max(1, 10 - static_cast<int>(index)); ++n) {
                LayoutPenaltyPair& currentLayout
                    = currentLayouts.emplace_back(startingLayout.layout, 0);
                LayoutMutator mutator{currentLayout.layout};
                mutator.performNSwapsOrCopies(numSwapsDistribution(generator), generator);
            }
        }

        /*for (auto const& startingLayout : currentLayouts) {
            LayoutPenaltyPair& currentLayout
                = currentLayouts.emplace_back(startingLayout.layout, 0);

            std::uniform_int_distribution<std::uint16_t> numSwapsDistribution{1, 5};
            LayoutMutator                                mutator{currentLayout.layout};
            mutator.performNSwapsOrCopies(numSwapsDistribution(generator), generator);
        }*/
        auto rng_end = std::chrono::high_resolution_clock::now();

        for (auto&& [worker, currentLayout] :
             ranges::views::zip(workers, currentLayouts | ranges::views::drop(N))) {
            worker.post(currentLayout);
        }

        using std::chrono::duration_cast;
        using std::chrono::milliseconds;

        for (auto& worker : workers)
            worker.blockWhileWaiting();

        auto sortStart = std::chrono::high_resolution_clock::now();
        ranges::actions::sort(currentLayouts, std::less{}, &LayoutPenaltyPair::penalty);
        auto sortEnd = std::chrono::high_resolution_clock::now();

        if (currentLayouts.front().penalty == 0.f) {
            printLayout(currentLayouts.front().layout);
            fflush(stdout);
            throw std::runtime_error("Penalty is null");
        }
        // fmt::println("Sorted layouts for {}", duration_cast<milliseconds>(sortEnd - sortStart));

        // Keep only the N best
        currentLayouts.erase(currentLayouts.begin() + N, currentLayouts.end());

        auto iterationEnd = std::chrono::high_resolution_clock::now();

        // fmt::println("RNG during {}", duration_cast<milliseconds>(rng_end - iterationStart));

        if (currentLayouts.front().penalty == lastBestPenalty) {
            ++countSameLayout;
        } else {
            countSameLayout = 0;
            lastBestPenalty = currentLayouts.front().penalty;
        }

        fmt::println(" = Iteration  {}\t{}\tbest penalty: {}", i,
                     std::chrono::duration_cast<milliseconds>(iterationEnd - iterationStart),
                     currentLayouts.front().penalty);
        std::fflush(stdout);

        if (countSameLayout > 2000) {
            // if (countSameLayout > 5) {
            for (auto& worker : workers)
                worker.stop();

            for (auto& thread : threads)
                thread.join();
            break;
        }
    }

    fmt::println("Out of the loop");
    return std::move(currentLayouts.front().layout);
}

float Generator::computeLayoutPenalty(TargetKeyboardLayout const& layout) const {
    KeyPressCalculator sim{layout};

    auto startSim  = std::chrono::high_resolution_clock::now();
    auto totalKeys = sim.simulate(corpus);
    auto endSim    = std::chrono::high_resolution_clock::now();

    /*std::cout << totalKeys.size() << " out of " << totalKeys.capacity() << "\t"
              << std::chrono::duration_cast<std::chrono::milliseconds>(endSim - startSim)
              << std::endl;*/

    // auto shortcutKeys = sim.simulateShortcuts(shortcuts);

    auto p1 = penaltiesForKeys(corpus.size(), totalKeys);
    // auto p2 = penaltiesForKeys(shortcuts.size(), shortcutKeys);

    return p1;
    // return std::max(p1, p2);
}

float Generator::penaltiesForKeys(std::size_t size, std::vector<KeyPress> const& totalKeys) {
    auto startNGraph = std::chrono::high_resolution_clock::now();
    auto ngraphSet   = NGraphSet::computeSetFromKeyPresses(totalKeys);
    auto endNGraph   = std::chrono::high_resolution_clock::now();
    /*std::cout << "ngraphs: " << ngraphSet.size() << "\t"
              << std::chrono::duration_cast<std::chrono::milliseconds>(endNGraph - startNGraph)
              << std::endl;*/

    PenaltyCalculator penaltyCalculator = PenaltyCalculator::defaultPenalties();
    auto              startPenalties    = std::chrono::high_resolution_clock::now();
    int64_t           penalty           = penaltyCalculator.computePenalties(ngraphSet);
    auto              endPenalties      = std::chrono::high_resolution_clock::now();
    float             normalizedPenalty = static_cast<float>(penalty) / static_cast<float>(size);
    /*std::cout << fmt::format(
        "penalty : {}, normalized: {} ({})", penalty, normalizedPenalty,
        std::chrono::duration_cast<std::chrono::milliseconds>(endPenalties - startPenalties))
              << std::endl;*/

    return normalizedPenalty;
}
} // namespace finger_tracking
