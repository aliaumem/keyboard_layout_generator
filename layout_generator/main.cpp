#include "layout_generator/simulator/simulator.hpp"

#include "voyager_shape.hpp"
#include "layout_generator/keyboard_layout.hpp"

#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"
#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"

#include <functional>
#include <iostream>

using namespace finger_tracking;

int main() {

    // clang-format off
    auto keys = std::array<Key, 52>{
        "Esc",  "&", "é", "\"",  "'", "(",    "-", "è", "_", "ç", "à",  ")",
        "CpLk", "a", "z", "e",  "r", "t",     "y", "u", "i", "o", "p",  "^",
        "LSft", "q", "s", "d",  "f", "g",     "h", "j", "k", "l", "m",  "ù",
          "<",  "w", "x", "c",  "v", "b",     "n", "," ,";", ":", "!", "RCtl",
                        "Spc",  "Tab",          "BkSp", "Enter"
    };
    // clang-format on

    KeyboardLayer  layerNormal{keys};
    KeyboardLayout layout{voyagerShape(), std::vector{layerNormal}};

    for (auto [key, position] : layout) {
        std::cout << std::format("{}   \t{}", key, position.center()) << std::endl;
    }

    auto      quartad = Quartad{{"i", "m", "f", "e"}};
    Simulator sim{layout};
    float     penalty = sim.computePenalties(std::vector{quartad});

    // - Compute the key sequence for the quartad
    // - Compute each penalty
    // - Do the annealing

    std::cout << std::format("penalty : {}", penalty) << std::endl;

    return 0;
}
