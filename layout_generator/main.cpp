#include "layout_generator/simulator/simulator.hpp"
#include "layout_generator/azerty_voyager_layout.hpp"

#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"
#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"
#include "layout_generator/penalties/penalty_calculator.hpp"

#include <functional>
#include <iostream>

using namespace finger_tracking;

int main() {

    // - Compute the key sequence for the quartad
    // - Compute each penalty
    // - Do the annealing
    try {
        auto layout = azertyVoyagerLayout();

        Simulator sim{layout};
        auto      totalKeys = sim.simulate("pqD&éa");
        std::cout << totalKeys.size() << std::endl;

        PenaltyCalculator penaltyCalculator{};
        float             penalty = penaltyCalculator.computePenalties(
            std::vector<Quartad>{{totalKeys[0], totalKeys[1], totalKeys[2], totalKeys[3]}});
        std::cout << std::format("penalty : {}", penalty) << std::endl;

    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
