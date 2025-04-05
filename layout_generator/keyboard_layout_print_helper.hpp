
#ifndef KEYBOARD_LAYOUT_PRINT_HELPER_HPP
#define KEYBOARD_LAYOUT_PRINT_HELPER_HPP

#include "layout_generator/keyboard_layout.hpp"
#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"

#include <fmt/ranges.h>
#include "range/v3/view/chunk.hpp"
#include "range/v3/view/transform.hpp"

namespace finger_tracking {
inline void printLayout(TargetKeyboardLayout resultLayout) {
    for (auto layer : resultLayout | ranges::views::chunk(52)) {

        for (auto row : layer | ranges::views::chunk(12)) {
            auto keys = row
                      | ranges::views::transform(
                            [&resultLayout](LayoutKeyRef keyRef) -> std::string_view {
                                return resultLayout.keyAt(keyRef).name;
                            });
            fmt::print("'{}'\n", fmt::join(keys, "' '"));
        }
        fmt::print("\n-----------------\n");
    }
}
} // namespace finger_tracking

#endif // KEYBOARD_LAYOUT_PRINT_HELPER_HPP
