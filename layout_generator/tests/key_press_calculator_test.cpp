#include "layout_generator/key_press_calculator/key_press_calculator.hpp"
#include "layout_generator/azerty_voyager_layout.hpp"

#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"
#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"

#include "layout_generator/layout_print_helpers.hpp"

#include "catch_string_helper.hpp"
#include "catch2/catch_test_macros.hpp"

using namespace finger_tracking;

SCENARIO("The KeyPressCalculator creates a sequence of key presses to output the desired key") {
    auto layout = azertyVoyagerLayout();

    KeyPressCalculator keyPressCalculator{layout};

    GIVEN("A key on the default layout") {
        Key  key{"é"};
        auto expected = LayoutKeyRef{0, HandSide::Left, Row::Fn, Column::Ring};

        WHEN("Starting from the default layout") {
            std::uint8_t layer    = 0;
            auto         sequence = keyPressCalculator.sequenceForKey(layer, key);

            THEN("There is only the key press found") {
                REQUIRE(sequence.size() == 1);
                CHECK(sequence.begin()->keyRef == expected);
            }
        }

        WHEN("Starting from another layout") {
            std::uint8_t layer               = 2;
            auto         sequence            = keyPressCalculator.sequenceForKey(layer, key);
            auto         transitionToDefault = KeyPress{
                LayoutKeyRef{0, HandSide::Right, Row::Thumb, Column::Index},
                Finger::Thumb,
                false,
            };

            THEN("We have a key release and then the normal key press") {
                REQUIRE(sequence.size() == 2);
                auto it = sequence.begin();
                CHECK(*it++ == transitionToDefault);
                CHECK(it->keyRef == expected);
            }
        }
    }

    GIVEN("A key on a non-default layout") {
        Key  key{"€"};
        auto expected = LayoutKeyRef{2, HandSide::Left, Row::Top, Column::Middle};

        WHEN("Starting from the same layout") {
            std::uint8_t layer    = 2;
            auto         sequence = keyPressCalculator.sequenceForKey(layer, key);

            THEN("There is only the key press found") {
                REQUIRE(sequence.size() == 1);
                CHECK(sequence.begin()->keyRef == expected);
            }
        }
        auto transitionFromDefault = KeyPress{
            /*.keyRef   = */ LayoutKeyRef{0, HandSide::Right, Row::Thumb, Column::Index},
            /*.finger   = */ Finger::Thumb,
            /*.isPress  = */ true,
        };

        WHEN("Starting from the default layout") {
            std::uint8_t layer    = 0;
            auto         sequence = keyPressCalculator.sequenceForKey(layer, key);

            THEN("We have a key press and then the normal key press") {
                REQUIRE(sequence.size() == 2);
                auto it = sequence.begin();
                CHECK(*it++ == transitionFromDefault);
                CHECK(it->keyRef == expected);
            }
        }

        WHEN("Starting from another non-default layout") {
            std::uint8_t layer               = 1;
            auto         sequence            = keyPressCalculator.sequenceForKey(layer, key);
            auto         transitionToDefault = KeyPress{
                /*.keyRef =*/LayoutKeyRef{0, HandSide::Left, Row::Home, Column::Middle},
                /*.finger   =*/Finger::Middle,
                /*.isPress  =*/false,
            };

            THEN("We have a key release, a key press and finally the normal key press") {
                REQUIRE(sequence.size() == 3);
                auto it = sequence.begin();
                CHECK(*it++ == transitionToDefault);
                CHECK(*it++ == transitionFromDefault);
                CHECK(it->keyRef == expected);
            }
        }
    }
}
