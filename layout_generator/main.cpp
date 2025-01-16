#include "layout_generator/simulator/simulator.hpp"
#include "layout_generator/azerty_voyager_layout.hpp"

#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"
#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"

#include <functional>
#include <iostream>

using namespace finger_tracking;

int main() {

    try {
        auto layout = azertyVoyagerLayout();

        Simulator sim{layout};
        float     penalty = sim.computePenalties(std::vector<Quartad>{
            {{"q", "q", "LSft", "f"}},
            {{"", "€", "Q", "LSft"}},
            {{"", "", "q", "q"}},
            {{"", "", "", "q"}},
        });

        // - Compute the key sequence for the quartad
        // - Compute each penalty
        // - Do the annealing

        std::cout << std::format("penalty : {}", penalty) << std::endl;
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
