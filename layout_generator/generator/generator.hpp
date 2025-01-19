#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "layout_generator/keyboard_layout.hpp"
#include "layout_generator/penalties/penalty_calculator.hpp"
#include "layout_generator/key_press_calculator/key_press_calculator.hpp"
#include "layout_generator/penalties/ngraphs.hpp"

namespace finger_tracking {
struct Generator {
    float penaltiesForKeys(std::size_t size, std::vector<KeyPress> const& totalKeys);

    float computeLayoutPenalty(std::string_view                          corpus,
                               std::vector<static_vector<Key, 4>> const& shortcuts,
                               TargetKeyboardLayout const&               layout);
};
} // namespace finger_tracking

#endif // GENERATOR_HPP
