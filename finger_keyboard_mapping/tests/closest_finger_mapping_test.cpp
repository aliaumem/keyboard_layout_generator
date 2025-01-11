#include "finger_keyboard_mapping/finger_to_key_mapping.hpp"
#include "print_helpers.hpp"
#include "catch2/catch_test_macros.hpp"

using namespace finger_tracking;

SCENARIO("A keyboard shape can map a finger to a key") {
    using namespace geo_literals;

    Key keyA{"A"};
    Key keyB{"B"};
    Key keyC{"C"};
    Key keyD{"D"};

    //  layout shape:   | A | B |
    //                  | C | D |
    KeyboardShape kb{static_vector{std::array<KeyInSpace, 62>{
        KeyInSpace{keyA, Rectangle{(0_x, 0_y), (9_w, 9_h)}},
        KeyInSpace{keyB, Rectangle{(10_x, 0_y), (9_w, 9_h)}},
        KeyInSpace{keyC, Rectangle{(0_x, 10_y), (9_w, 9_h)}},
        KeyInSpace{keyD, Rectangle{(10_x, 10_y), (9_w, 9_h)}},
    }}};

    GIVEN("No hand") {
        BothHands hands{};

        THEN("There is no matching finger") {
            CHECK(kb.closestFinger(keyA, hands) == std::nullopt);
            CHECK(kb.closestFinger(keyB, hands) == std::nullopt);
            CHECK(kb.closestFinger(keyC, hands) == std::nullopt);
            CHECK(kb.closestFinger(keyD, hands) == std::nullopt);
        }
    }

    GIVEN("A hand with only one finger close") {
        BothHands hands{
            .left = Hand{
                .thumb  = (100_x, 100_y),
                .index  = (5_x, 5_y),
                .middle = (100_x, 100_y),
                .ring   = (100_x, 100_y),
                .pinky  = (100_x, 100_y),
            },
        };

        THEN("The matching finger is always the same") {
            using enum FingerDesc::Finger;
            using enum FingerDesc::Side;
            CHECK(kb.closestFinger(keyA, hands) == FingerRef{{Left, Index}, (5_x, 5_y)});
            CHECK(kb.closestFinger(keyB, hands) == FingerRef{{Left, Index}, (5_x, 5_y)});
            CHECK(kb.closestFinger(keyC, hands) == FingerRef{{Left, Index}, (5_x, 5_y)});
            CHECK(kb.closestFinger(keyD, hands) == FingerRef{{Left, Index}, (5_x, 5_y)});
        }
    }

    GIVEN("A hand with one close finger on each side") {
        BothHands hands{
            .left = Hand{
                .thumb  = (100_x, 100_y),
                .index  = (-3_x, 5_y),
                .middle = (100_x, 100_y),
                .ring   = (15_x, 5_y),
                .pinky  = (100_x, 100_y),
            },
        };

        THEN("The matching finger is always the same") {
            using enum FingerDesc::Finger;
            using enum FingerDesc::Side;
            CHECK(kb.closestFinger(keyA, hands) == FingerRef{{Left, Index}, (-3_x, 5_y)});
            CHECK(kb.closestFinger(keyB, hands) == FingerRef{{Left, Ring}, (15_x, 5_y)});
            CHECK(kb.closestFinger(keyC, hands) == FingerRef{{Left, Index}, (-3_x, 5_y)});
            CHECK(kb.closestFinger(keyD, hands) == FingerRef{{Left, Ring}, (15_x, 5_y)});
        }
    }

    GIVEN("Two hands with one close finger on each side") {
        BothHands hands{
            .left = Hand{
                .thumb  = (100_x, 100_y),
                .index  = (-3_x, 5_y),
                .middle = (100_x, 100_y),
                .ring = (100_x, 100_y),
                .pinky  = (100_x, 100_y),
            },
            .right = Hand {
                .thumb  = (100_x, 100_y),
                .index   = (15_x, 5_y),
                .middle  = (100_x, 100_y),
                .ring  = (100_x, 100_y),
                .pinky  = (100_x, 100_y),
            },
        };

        THEN("The matching finger is always the same") {
            using enum FingerDesc::Finger;
            using enum FingerDesc::Side;
            CHECK(kb.closestFinger(keyA, hands) == FingerRef{{Left, Index}, (-3_x, 5_y)});
            CHECK(kb.closestFinger(keyB, hands) == FingerRef{{Right, Index}, (15_x, 5_y)});
            CHECK(kb.closestFinger(keyC, hands) == FingerRef{{Left, Index}, (-3_x, 5_y)});
            CHECK(kb.closestFinger(keyD, hands) == FingerRef{{Right, Index}, (15_x, 5_y)});
        }
    }
}
