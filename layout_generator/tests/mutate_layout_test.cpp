#include "layout_generator/mutation/layout_mutator.hpp"
#include "layout_generator/azerty_voyager_layout.hpp"

#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"

#include "catch_string_helper.hpp"

#include "catch2/catch_test_macros.hpp"

using namespace finger_tracking;

SCENARIO("Keyboard layouts can be mutated") {
    auto layout = azertyVoyagerLayout();

    LayoutMutator mutator{layout};

    GIVEN("Keys on the same layer") {
        REQUIRE_FALSE(layout.areRelatedLayers(0, 3));

        LayoutKeyRef lhs{0, HandSide::Left, Row::Fn, Column::Pinky};
        LayoutKeyRef rhs{0, HandSide::Right, Row::Fn, Column::Pinky};

        THEN("The keys are swapped") {
            auto oldValueLhs = layout.keyAt(lhs);
            auto oldValueRhs = layout.keyAt(rhs);

            mutator.swapKeys(lhs, rhs);

            CHECK(oldValueLhs == layout.keyAt(rhs));
            CHECK(oldValueRhs == layout.keyAt(lhs));
        }
    }

    GIVEN("A key on the Shift layer") {
        LayoutKeyRef lhs{1, HandSide::Left, Row::Fn, Column::Middle};
        THEN("It can be moved to layer 5") {
            LayoutKeyRef rhs{5, HandSide::Left, Row::Home, Column::Middle};
            CHECK_FALSE(mutator.canSwapKeys(lhs, rhs));
            CHECK(mutator.canCopyTo(rhs));
            CHECK_FALSE(mutator.canCopyTo(lhs));

            auto oldKey = layout.keyAt(lhs);
            mutator.copyKeyTo(lhs, rhs);
            CHECK(layout.keyAt(lhs) == layout.keyAt(rhs));
            CHECK(layout.keyAt(lhs) == oldKey);
        }

        THEN("It can be moved to layer 0 on a repeated key") {
            LayoutKeyRef rhs{0, HandSide::Right, Row::Fn, Column::IndexExt};
            CHECK_FALSE(mutator.canSwapKeys(lhs, rhs));
            CHECK(mutator.canCopyTo(rhs));
            CHECK_FALSE(mutator.canCopyTo(lhs));

            auto oldKey = layout.keyAt(lhs);
            mutator.copyKeyTo(lhs, rhs);
            CHECK(layout.keyAt(lhs) == layout.keyAt(rhs));
            CHECK(layout.keyAt(lhs) == oldKey);
        }
    }
}
