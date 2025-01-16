#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "layout_generator/layout_key_ref.hpp"
#include "finger_keyboard_mapping/static_vector.hpp"
#include "finger_keyboard_mapping/keyboard/key.hpp"

#include <numeric>
#include <vector>
#include <functional>

namespace finger_tracking {
template <size_t N>
class KeyboardLayout;

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

struct KeyLayoutSequence {
    using value_type = LayoutKeyRef;
    static_vector<value_type, 8> keyPresses;

    value_type& emplace_back(value_type it) { return keyPresses.emplace_back(it); }

    [[nodiscard]] auto size() const { return keyPresses.size(); }
    [[nodiscard]] auto begin() const { return keyPresses.begin(); }
    [[nodiscard]] auto end() const { return keyPresses.end(); }
    [[nodiscard]] auto back() const { return keyPresses.back(); }
    [[nodiscard]] auto front() const { return keyPresses.front(); }
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
    Simulator(KeyboardLayout<52> const&);

    float computePenalties(std::vector<Quartad> const& quartads) const;

    [[nodiscard]] KeyLayoutSequence sequenceForQuartad(Quartad const& quartad) const;

    KeyboardLayout<52> const&                                   m_layout;
    std::vector<std::function<float(KeyLayoutSequence const&)>> m_penalties;
};
} // namespace finger_tracking

#endif // SIMULATOR_HPP
