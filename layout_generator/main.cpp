#include "layout_generator/simulator/simulator.hpp"
#include "layout_generator/azerty_voyager_layout.hpp"

#include "layout_generator/penalties/penalty_calculator.hpp"
#include "penalties/ngraphs.hpp"

#include <format>
#include <iostream>

using namespace finger_tracking;

int main() {

    // - Compute the key sequence for the quartad ✅
    // - Compute registered penalties ✅
    // - Fill in penalties 🔃
    // - Do the annealing 🕐

    try {
        auto layout = azertyVoyagerLayout();

        Simulator sim{layout};
        auto      totalKeys = sim.simulate("pqD&éa");
        std::cout << totalKeys.size() << std::endl;

        auto quartadSet = NGraphSet::computeSetFromKeyPresses(totalKeys);

        PenaltyCalculator penaltyCalculator = PenaltyCalculator::defaultPenalties();
        float             penalty           = penaltyCalculator.computePenalties(quartadSet);
        std::cout << std::format("penalty : {}", penalty) << std::endl;

    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
