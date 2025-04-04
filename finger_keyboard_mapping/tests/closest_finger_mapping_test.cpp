#include "finger_keyboard_mapping/mapping/closest_finger_mapping.hpp"
#include "print_helpers.hpp"
#include "catch2/catch_test_macros.hpp"

using namespace finger_tracking;

namespace finger_tracking {
bool operator==(MaybeFinger const& lhs, std::optional<FingerRef> const& rhs) {
    return lhs.maybeFinger == rhs;
}
bool operator==(MaybeFinger const& lhs, std::nullopt_t rhs) {
    return lhs.maybeFinger == rhs;
}
bool operator==(MaybeFinger const& lhs, FingerRef const& rhs) {
    return lhs.maybeFinger == rhs;
}
} // namespace finger_tracking

SCENARIO("A keyboard shape can map a finger to a key") {
    using namespace geo_literals;

    Key keyA{"A"};
    Key keyB{"B"};
    Key keyC{"C"};
    Key keyD{"D"};

    //  layout shape:   | A | B |
    //                  | C | D |
    KeyboardShape kb{static_vector<KeyInSpace, 62>{std::array{
        KeyInSpace{keyA, Rectangle{(0_x, 0_y), (9_w, 9_h)}},
        KeyInSpace{keyB, Rectangle{(10_x, 0_y), (9_w, 9_h)}},
        KeyInSpace{keyC, Rectangle{(0_x, 10_y), (9_w, 9_h)}},
        KeyInSpace{keyD, Rectangle{(10_x, 10_y), (9_w, 9_h)}},
    }}};

    GIVEN("No hand") {
        BothHands hands{};

        THEN("There is no matching finger") {
            CHECK(closestFinger(kb, keyA, hands) == std::nullopt);
            CHECK(closestFinger(kb, keyB, hands) == std::nullopt);
            CHECK(closestFinger(kb, keyC, hands) == std::nullopt);
            CHECK(closestFinger(kb, keyD, hands) == std::nullopt);
        }
    }

    GIVEN("A hand with only one finger close") {
        BothHands hands{
            .left = Hand{
                .thumb  = (100_x, 100_y),
                .index  = (1_x, 3_y),
                .middle = (100_x, 100_y),
                .ring   = (100_x, 100_y),
                .pinky  = (100_x, 100_y),
            },
        };

        THEN("The matching finger is always the same") {
            using enum FingerDesc::Finger;
            using enum FingerDesc::Side;
            CHECK(closestFinger(kb, keyA, hands) == hands[{Left, Index}]);
            CHECK(closestFinger(kb, keyB, hands) == std::nullopt);
            CHECK(closestFinger(kb, keyC, hands) == hands[{Left, Index}]);
            CHECK(closestFinger(kb, keyD, hands) == std::nullopt);
        }
    }

    GIVEN("A hand with one close finger on each side") {
        BothHands hands{
            .left = Hand{
                .thumb  = (100_x, 100_y),
                .index  = (1_x, 3_y),
                .middle = (100_x, 100_y),
                .ring   = (15_x, 5_y),
                .pinky  = (100_x, 100_y),
            },
        };

        THEN("The matching finger is always the same") {
            using enum FingerDesc::Finger;
            using enum FingerDesc::Side;
            CHECK(closestFinger(kb, keyA, hands) == hands[{Left, Index}]);
            CHECK(closestFinger(kb, keyB, hands) == hands[{Left, Ring}]);
            CHECK(closestFinger(kb, keyC, hands) == hands[{Left, Index}]);
            CHECK(closestFinger(kb, keyD, hands) == hands[{Left, Ring}]);
        }
    }

    GIVEN("Two hands with one close finger on each side") {
        BothHands hands{
            .left = Hand{
                .thumb  = (100_x, 100_y),
                .index  = (5_x, 5_y),
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
            CHECK(closestFinger(kb, keyA, hands) == hands[{Left, Index}]);
            CHECK(closestFinger(kb, keyB, hands) == hands[{Right, Index}]);
            CHECK(closestFinger(kb, keyC, hands) == hands[{Left, Index}]);
            CHECK(closestFinger(kb, keyD, hands) == hands[{Right, Index}]);
        }
    }
}
