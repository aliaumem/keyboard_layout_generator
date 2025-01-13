#ifndef MAYBE_FINGER_HPP
#define MAYBE_FINGER_HPP

#include "finger_keyboard_mapping/hands/both_hands.hpp"

#include <optional>

namespace finger_tracking {
struct MaybeFinger {
    std::optional<FingerRef> maybeFinger;
    int                      distance;

    bool  has_value() const { return maybeFinger.has_value(); }
    auto& value() const { return maybeFinger.value(); }
    auto& operator*() const { return *maybeFinger; }
    auto* operator->() const { return maybeFinger.operator->(); }

    static constexpr MaybeFinger nullopt() { return MaybeFinger{std::nullopt, 1000}; }
};
} // namespace finger_tracking

#endif // MAYBE_FINGER_HPP
