#include "finger_keyboard_mapping/mapping_geometry.hpp"
#include "print_helpers.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace finger_tracking;
using namespace finger_tracking::geo_literals;

SCENARIO("Hands can be iterated on and iterators dereferenced") {
    GIVEN("Both empty hands") {
        BothHands hands{};

        THEN("There is nothing to iterate on") {
            CHECK(hands.begin() == hands.end());
            CHECK(hands.size() == 0);
        }
    }

    GIVEN("A left hand") {
        BothHands hands{.left = Hand{
                            .thumb  = (0_x, 0_y),
                            .index  = (1_x, 1_y),
                            .middle = (2_x, 2_y),
                            .ring   = (3_x, 3_y),
                            .pinky  = (4_x, 4_y),
                        }};

        THEN("There is something to iterate on") {
            CHECK(hands.begin() != hands.end());
            CHECK(hands.size() == 5);
        }

        WHEN("Iterating over the fingers 5 times") {
            auto it = hands.begin();
            for (int i = 0; i < 5; i++)
                ++it;

            THEN("We have arrived at the end") {
                CHECK(it == hands.end());
            }
        }

        WHEN("Iterating over each finger") {
            auto it = hands.begin();
            THEN("We get the fingers in order starting at the thumb") {
                using enum FingerDesc::Finger;
                using enum FingerDesc::Side;
                CHECK(*it == FingerDesc{Left, Thumb, hands.left->thumb});
                ++it;
                CHECK(*it == FingerDesc{Left, Index, hands.left->index});
                ++it;
                CHECK(*it == FingerDesc{Left, Middle, hands.left->middle});
                ++it;
                CHECK(*it == FingerDesc{Left, Ring, hands.left->ring});
                ++it;
                CHECK(*it == FingerDesc{Left, Pinky, hands.left->pinky});
            }
        }
    }

    GIVEN("A right hand") {
        BothHands hands{.right = Hand{
                            .thumb  = (0_x, 0_y),
                            .index  = (1_x, 1_y),
                            .middle = (2_x, 2_y),
                            .ring   = (3_x, 3_y),
                            .pinky  = (4_x, 4_y),
                        }};

        THEN("There is something to iterate on") {
            CHECK(hands.begin() != hands.end());
            CHECK(hands.size() == 5);
        }

        WHEN("Iterating over the fingers 5 times") {
            auto it = hands.begin();
            for (int i = 0; i < 5; i++)
                ++it;

            THEN("We have arrived at the end") {
                CHECK(it == hands.end());
            }
        }

        WHEN("Iterating over each finger") {
            auto it = hands.begin();
            THEN("We get the fingers in order starting at the thumb") {
                using enum FingerDesc::Finger;
                using enum FingerDesc::Side;
                CHECK(*it == FingerDesc{Right, Thumb, hands.right->thumb});
                ++it;
                CHECK(*it == FingerDesc{Right, Index, hands.right->index});
                ++it;
                CHECK(*it == FingerDesc{Right, Middle, hands.right->middle});
                ++it;
                CHECK(*it == FingerDesc{Right, Ring, hands.right->ring});
                ++it;
                CHECK(*it == FingerDesc{Right, Pinky, hands.right->pinky});
            }
        }
    }

    GIVEN("Both hands") {
        BothHands hands{
                    .left = Hand{
                        .thumb  = (0_x, 0_y),
                        .index  = (1_x, 1_y),
                        .middle = (2_x, 2_y),
                        .ring   = (3_x, 3_y),
                        .pinky  = (4_x, 4_y),
                    },
                    .right = Hand{
                        .thumb  = (0_x, 0_y),
                        .index  = (1_x, 1_y),
                        .middle = (2_x, 2_y),
                        .ring   = (3_x, 3_y),
                        .pinky  = (4_x, 4_y),
                    },
                };

        THEN("There is something to iterate on") {
            CHECK(hands.begin() != hands.end());
            CHECK(hands.size() == 10);
        }

        WHEN("Iterating over the fingers 10 times") {
            auto it = hands.begin();
            for (int i = 0; i < 10; i++)
                ++it;

            THEN("We have arrived at the end") {
                CHECK(it == hands.end());
            }
        }

        WHEN("Iterating over each finger") {
            auto it = hands.begin();
            THEN("We get the fingers in order starting at the thumb") {
                using enum FingerDesc::Finger;
                using enum FingerDesc::Side;
                CHECK(*it == FingerDesc{Left, Thumb, hands.left->thumb});
                ++it;
                CHECK(*it == FingerDesc{Left, Index, hands.left->index});
                ++it;
                CHECK(*it == FingerDesc{Left, Middle, hands.left->middle});
                ++it;
                CHECK(*it == FingerDesc{Left, Ring, hands.left->ring});
                ++it;
                CHECK(*it == FingerDesc{Left, Pinky, hands.left->pinky});
                ++it;
                CHECK(*it == FingerDesc{Right, Thumb, hands.right->thumb});
                ++it;
                CHECK(*it == FingerDesc{Right, Index, hands.right->index});
                ++it;
                CHECK(*it == FingerDesc{Right, Middle, hands.right->middle});
                ++it;
                CHECK(*it == FingerDesc{Right, Ring, hands.right->ring});
                ++it;
                CHECK(*it == FingerDesc{Right, Pinky, hands.right->pinky});
            }
        }
    }
}
