#ifndef KEY_PRESS_CALCULATOR_HPP
#define KEY_PRESS_CALCULATOR_HPP

#include "layout_generator/key_press.hpp"
#include "finger_keyboard_mapping/static_vector.hpp"
#include "finger_keyboard_mapping/keyboard/key.hpp"
#include "layout_generator/keyboard_layout.hpp"

#include <vector>

namespace finger_tracking {

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
    [[nodiscard]] auto front() const -> value_type const& { return keyPresses[0]; }
    [[nodiscard]] auto back() const -> value_type const& { return keyPresses[size() - 1]; }
};

class KeyPressCalculator {
public:
    explicit KeyPressCalculator(TargetKeyboardLayout const& layout);

    [[nodiscard]] KeyLayoutSequence     sequenceForKey(std::uint8_t& layer, Key const& key) const;
    [[nodiscard]] std::vector<KeyPress> simulate(std::string_view corpus) const;
    [[nodiscard]] std::vector<KeyPress> simulateShortcuts(
        std::vector<static_vector<Key, 4>> const& shortcuts) const;

private:
    auto lookupKey(Key const& key) const -> LayoutKeyRef;
    void emplaceKeyRefInSequence(KeyLayoutSequence& sequence, LayoutKeyRef keyRef,
                                 bool isPress = true) const;
    void insertLayoutChangeSequence(std::uint8_t fromLayer, KeyLayoutSequence& sequence,
                                    LayoutKeyRef keyRef) const;
    TargetKeyboardLayout const& m_layout;
    struct ReverseLookupInfo {
        Key    key;
        size_t index;
        bool   isHeldKey;
    };
    std::vector<ReverseLookupInfo> m_reverseLookup;
};
} // namespace finger_tracking

#endif // KEY_PRESS_CALCULATOR_HPP
