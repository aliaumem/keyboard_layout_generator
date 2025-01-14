#ifndef KEY_HPP
#define KEY_HPP

#include <string_view>

namespace finger_tracking {
struct Key {
    std::string_view name;

    bool operator==(Key const&) const = default;

    [[nodiscard]] bool isValid() const { return !name.empty(); }

    static Key invalid;
};

inline Key Key::invalid{};
} // namespace finger_tracking

#endif // KEY_HPP
