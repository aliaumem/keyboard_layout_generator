#include "layout_generator/simulator/simulator.hpp"
#include "layout_generator/azerty_voyager_layout.hpp"

#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"
#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"

template <>
struct std::formatter<finger_tracking::LayoutKeyRef> : std::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::LayoutKeyRef value, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}. {} {}:{}", value.layer, value.side, value.row,
                              value.column);
    }
};

template <>
struct std::formatter<finger_tracking::KeyPress> : std::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::KeyPress const& value, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{} ({}) {} @{}", value.keyRef, value.finger,
                              value.isPress, value.position);
    }
};

#include "catch_string_helper.hpp"
#include "catch2/catch_test_macros.hpp"

using namespace finger_tracking;

SCENARIO("The simulator creates a sequence of key presses to output the desired key") {
    auto layout = azertyVoyagerLayout();

    Simulator simulator{layout};

    GIVEN("A key on the default layout") {
        Key  key{"é"};
        auto expected = LayoutKeyRef{.layer = 0, .side = HandSide::Left, .row = Fn, .column = Ring};

        WHEN("Starting from the default layout") {
            auto sequence = simulator.sequenceForKey(0, key);

            THEN("There is only the key press found") {
                REQUIRE(sequence.size() == 1);
                CHECK(sequence.begin()->keyRef == expected);
            }
        }

        WHEN("Starting from another layout") {
            auto sequence            = simulator.sequenceForKey(2, key);
            auto transitionToDefault = KeyPress{
                .keyRef   = LayoutKeyRef{.layer  = 0,
                                         .side   = HandSide::Right,
                                         .row    = Thumb,
                                         .column = IndexExt},
                .finger   = Finger::Thumb,
                .isPress  = false,
                .position = Point16{174, 94},
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
        auto expected
            = LayoutKeyRef{.layer = 2, .side = HandSide::Left, .row = Top, .column = Middle};

        WHEN("Starting from the same layout") {
            auto sequence = simulator.sequenceForKey(2, key);

            THEN("There is only the key press found") {
                REQUIRE(sequence.size() == 1);
                CHECK(sequence.begin()->keyRef == expected);
            }
        }
        auto transitionFromDefault = KeyPress{
            .keyRef
            = LayoutKeyRef{.layer = 0, .side = HandSide::Right, .row = Thumb, .column = IndexExt},
            .finger   = Finger::Thumb,
            .isPress  = true,
            .position = Point16{174, 94},
        };

        WHEN("Starting from the default layout") {
            auto sequence = simulator.sequenceForKey(0, key);

            THEN("We have a key press and then the normal key press") {
                REQUIRE(sequence.size() == 2);
                auto it = sequence.begin();
                CHECK(*it++ == transitionFromDefault);
                CHECK(it->keyRef == expected);
            }
        }

        WHEN("Starting from another non-default layout") {
            auto sequence            = simulator.sequenceForKey(1, key);
            auto transitionToDefault = KeyPress{
                .keyRef
                = LayoutKeyRef{.layer = 0, .side = HandSide::Right, .row = Home, .column = Middle},
                .finger   = Finger::Middle,
                .isPress  = false,
                .position = Point16{221, 9},
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
