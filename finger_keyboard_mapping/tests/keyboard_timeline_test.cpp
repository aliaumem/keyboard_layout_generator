#include "finger_keyboard_mapping/keyboard_timeline.hpp"

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_vector.hpp"

using finger_tracking::FingerDesc;
using finger_tracking::Key;
using finger_tracking::KeyboardState;
using finger_tracking::KeyboardTimeline;

SCENARIO("The timeline represents the state of the keyboard at each frame") {
    KeyboardTimeline::Builder builder;

    using enum FingerDesc::Side;
    using enum FingerDesc::Finger;

    builder.pressed(Key{"f"}, FingerDesc{Left, Index}).nextFrame().released(Key{"f"});

    KeyboardTimeline timeline = builder.build();

    auto expected = std::vector<std::pair<Key, FingerDesc>>{{Key{"f"}, FingerDesc{Left, Index}}};
    CHECK_THAT(timeline[0].pressedKeys, Catch::Matchers::Equals(expected));
    CHECK_THAT(timeline[1].pressedKeys,
               Catch::Matchers::Equals(std::vector<std::pair<Key, FingerDesc>>{}));
}
