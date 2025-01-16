#include "layout_generator/simulator/simulator.hpp"

#include "voyager_shape.hpp"
#include "layout_generator/keyboard_layout.hpp"

#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"
#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"

#include <functional>
#include <iostream>

using namespace finger_tracking;

int main() {

    try {
        // clang-format off
        auto lowerKeys = std::array<Key, 52>{
            "Esc",  "&", "é", "\"",  "'", "(",    "-", "è", "_", "ç", "à",  ")",
            "CpLk", "a", "z", "e",  "r", "t",     "y", "u", "i", "o", "p",  "^",
            "LSft", "q", "s", "d",  "f", "g",     "h", "j", "k", "l", "m",  "ù",
              "<",  "w", "x", "c",  "v", "b",     "n", ",", ";", ":", "!", "RCtl",
                               "Spc",  "Tab",     "BkSp", "Enter"
        };
        auto upperKeys = std::array<Key, 52>{
            "Esc",  "1", "2", "3",  "4", "5",     "6", "7", "8", "9", "0",  "+",
            "CpLk", "A", "Z", "E",  "R", "T",     "Y", "U", "I", "O", "P",  "¨",
            "LSft", "Q", "S", "D",  "F", "G",     "H", "J", "K", "L", "M",  "%",
              ">",  "W", "X", "C",  "V", "B",     "N", "?", ".", "/", "§", "RCtl",
                               "Spc",  "Tab",     "BkSp", "Enter"
        };
        // clang-format on

        KeyboardLayer  layerNormal{lowerKeys};
        KeyboardLayout layout{voyagerShape(), std::vector{layerNormal, {upperKeys}}};

        for (auto [key, position] : layout) {
            std::cout << std::format("{}   \t{}", key, position.center()) << std::endl;
        }

        Simulator sim{layout};
        float     penalty = sim.computePenalties(std::vector<Quartad>{
            {{"q", "q", "LSft", "f"}},
            {{"", "q", "Q", "LSft"}},
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
