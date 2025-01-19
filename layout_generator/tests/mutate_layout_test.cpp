#include "layout_generator/mutation/layout_mutator.hpp"
#include "layout_generator/azerty_voyager_layout.hpp"

#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"

#include "catch_string_helper.hpp"

#include "catch2/catch_test_macros.hpp"

using namespace finger_tracking;

SCENARIO("Keyboard layouts can be mutated") {
    auto layout = azertyVoyagerLayout();

    LayoutMutator mutator{layout};

    LayoutKeyRef lhs{0, HandSide::Left, Row::Fn, Column::Pinky};
    LayoutKeyRef rhs{1, HandSide::Right, Row::Fn, Column::Pinky};

    auto oldValueLhs = layout.keyAt(lhs);
    auto oldValueRhs = layout.keyAt(rhs);

    mutator.swapKeys(lhs, rhs);

    CHECK(oldValueLhs.first == layout.keyAt(rhs).first);
    CHECK(oldValueRhs.first == layout.keyAt(lhs).first);
}
