#ifndef KEY_HPP
#define KEY_HPP

#include <string_view>

namespace finger_tracking {
struct Key {
    std::string_view name;

    std::strong_ordering operator<=>(Key const&) const = default;

    [[nodiscard]] bool isValid() const { return !name.empty(); }
    [[nodiscard]] bool isEmpty() const { return name.empty(); }
    [[nodiscard]] bool isUpperCaseLetter() const {
        return name.size() == 1 && 'A' <= name.front() && name.front() <= 'Z';
    }

    static Key invalid;

    template <typename H>
    friend H AbslHashValue(H h, Key const& key);
};

inline Key Key::invalid{};

template <typename H>
H AbslHashValue(H h, Key const& key) {
    return H::combine(std::move(h), key.name);
}
} // namespace finger_tracking

#endif // KEY_HPP
