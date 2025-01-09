#ifndef KEY_HPP
#define KEY_HPP

#include <string_view>

namespace finger_tracking {
struct Key {
    std::string_view name;

    bool operator==(Key const&) const = default;
};
} // namespace finger_tracking

#endif // KEY_HPP
