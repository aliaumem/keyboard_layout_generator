#include "finger_keyboard_mapping/keyboard/key.hpp"

#include "absl/hash/hash_testing.h"
#include "absl/container/flat_hash_map.h"

#include "catch2/catch_test_macros.hpp"

using namespace finger_tracking;

SCENARIO("Keys are hashable") {

    CHECK(absl::VerifyTypeImplementsAbslHashCorrectly({Key{"K"}, Key{"Esc"}, Key{"ç"}}));

    absl::flat_hash_map<Key, Key> keys{};

    STATIC_REQUIRE(absl::hash_internal::is_hashable<finger_tracking::Key>::value);
    STATIC_REQUIRE(absl::container_internal::memory_internal::IsLayoutCompatible<
                   finger_tracking::Key, finger_tracking::Key>::value);

    STATIC_REQUIRE_FALSE(absl::container_internal::FlatHashMapPolicy<Key, Key>::get_hash_slot_fn<
                             absl::hash_internal::Hash<finger_tracking::Key>>()
                         == nullptr);
}