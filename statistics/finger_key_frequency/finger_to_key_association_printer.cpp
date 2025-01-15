#include "finger_to_key_association.hpp"

#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"
#include "range/v3/view/enumerate.hpp"

#include <iostream>

namespace finger_tracking {
void FingerToKeyAssociation::print(FingerToKeyMap const& association) const {
    for (auto const& [i, fingers] : ranges::views::enumerate(association)) {
        std::cout << keyMap.keys[i].name << "\t";
        for (auto const& [fingerInt, finger] : ranges::views::enumerate(fingers)) {
            if (finger != 0) {
                FingerDesc desc{static_cast<int>(fingerInt)};
                std::cout << std::format("[{}: {}]", desc, finger);
            }
        }
        std::cout << std::endl;
    }
}
} // namespace finger_tracking
