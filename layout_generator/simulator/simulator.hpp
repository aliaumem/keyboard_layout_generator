#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "finger_keyboard_mapping/mapping_geometry.hpp"
#include "layout_generator/layout_key_ref.hpp"
#include "finger_keyboard_mapping/static_vector.hpp"
#include "finger_keyboard_mapping/keyboard/key.hpp"
#include "finger_keyboard_mapping/hands/finger_desc.hpp"

#include <vector>
#include <functional>

namespace finger_tracking {
template <size_t N>
class KeyboardLayout;
using TargetKeyboardLayout = KeyboardLayout<52>;

enum class Row : std::uint8_t {
    Fn = 0,
    Top,
    Home,
    Bottom,
    Thumb
};

enum class Column : std::uint8_t {
    IndexExt = 0,
    Index,
    Middle,
    Ring,
    Pinky,
    PinkyExt,
};

struct KeyPress {
    LayoutKeyRef keyRef;
    Finger       finger;
    Point        position;

    [[nodiscard]] HandSide   side() const { return keyRef.side; }
    [[nodiscard]] FingerDesc fingerDesc() const { return {side(), finger}; }
};

struct KeyLayoutSequence {
    using value_type = KeyPress;
    static_vector<value_type, 8> keyPresses;

    template <typename... Args>
        requires std::constructible_from<value_type, Args...>
    value_type& emplace_back(Args&&... args) {
        return keyPresses.emplace_back(std::forward<Args>(args)...);
    }

    auto current() const { return keyPresses.front(); }
    auto prev1() const { return keyPresses[1]; }

    [[nodiscard]] auto size() const { return keyPresses.size(); }
    [[nodiscard]] auto begin() const { return keyPresses.begin(); }
    [[nodiscard]] auto end() const { return keyPresses.end(); }
};

struct Quartad {
    std::array<Key, 4> keys;
    Key                current() const { return keys[3]; }
    Key                prev1() const { return keys[2]; }
    Key                prev2() const { return keys[1]; }
    Key                prev3() const { return keys[0]; }

    auto begin() const { return keys.begin(); }
    auto end() const { return keys.end(); }
};

struct Simulator {
    Simulator(TargetKeyboardLayout const&);

    float computePenalties(std::vector<Quartad> const& quartads) const;

    [[nodiscard]] KeyLayoutSequence sequenceForQuartad(Quartad const& quartad) const;

    TargetKeyboardLayout const&                                 m_layout;
    std::vector<std::function<float(KeyLayoutSequence const&)>> m_penalties;
};
} // namespace finger_tracking

#endif // SIMULATOR_HPP
