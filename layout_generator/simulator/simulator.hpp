#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "finger_keyboard_mapping/mapping_geometry.hpp"
#include "layout_generator/layout_key_ref.hpp"
#include "finger_keyboard_mapping/static_vector.hpp"
#include "finger_keyboard_mapping/keyboard/key.hpp"
#include "finger_keyboard_mapping/hands/finger_desc.hpp"
#include "layout_generator/keyboard_layout.hpp"

#include <vector>

namespace finger_tracking {

struct KeyPress {
    LayoutKeyRef keyRef;
    Finger       finger;
    Point        position;
    bool         isPress = true;

    [[nodiscard]] HandSide   side() const { return keyRef.side; }
    [[nodiscard]] FingerDesc fingerDesc() const { return {side(), finger}; }

    bool operator==(KeyPress const& other) const = default;
};

struct KeyLayoutSequence {
    using value_type = KeyPress;
    static_vector<value_type, 8> keyPresses;

    template <typename... Args>
        requires std::constructible_from<value_type, Args...>
    value_type& emplace_back(Args&&... args) {
        return keyPresses.emplace_back(std::forward<Args>(args)...);
    }

    [[nodiscard]] auto size() const { return keyPresses.size(); }
    [[nodiscard]] auto begin() const { return keyPresses.begin(); }
    [[nodiscard]] auto end() const { return keyPresses.end(); }
};

struct Quartad {
    std::array<Key, 4> keys;
    auto               begin() const { return keys.begin(); }
    auto               end() const { return keys.end(); }
};

class Simulator {
public:
    explicit Simulator(TargetKeyboardLayout const&);

    float computePenalties(std::vector<Quartad> const& quartads) const;

    [[nodiscard]] KeyLayoutSequence sequenceForKey(std::uint8_t layer, Key const& key) const;
    [[nodiscard]] KeyLayoutSequence sequenceForQuartad(Quartad const& quartad) const;

private:
    void emplaceKeyRefInSequence(KeyLayoutSequence& sequence, LayoutKeyRef keyRef,
                                 bool isPress = true) const;
    void insertLayoutChangeSequence(std::uint8_t fromLayer, KeyLayoutSequence& sequence,
                                    LayoutKeyRef keyRef) const;
    TargetKeyboardLayout const&                      m_layout;
    std::vector<float (*)(KeyLayoutSequence const&)> m_penalties;
};
} // namespace finger_tracking

#endif // SIMULATOR_HPP
