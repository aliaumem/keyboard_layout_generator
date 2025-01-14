#ifndef BOTH_HANDS_HPP
#define BOTH_HANDS_HPP

#include "finger_keyboard_mapping/mapping_geometry.hpp"
#include "finger_keyboard_mapping/hands/finger_desc.hpp"

#include <optional>
#include <ranges>

namespace finger_tracking {

struct FingerRef {
    FingerDesc fingerDesc;
    Point      position;
    bool       operator==(FingerRef const& other) const = default;
};

struct BothHands {
    struct iterator {
        using value_type        = FingerRef;
        using reference         = FingerRef;
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;

        int              index = 0;
        BothHands const* hands;

        FingerRef operator*() const {
            Point position = [this]() {
                switch (index) {
                case 0: return hands->left->thumb;
                case 1: return hands->left->index;
                case 2: return hands->left->middle;
                case 3: return hands->left->ring;
                case 4: return hands->left->pinky;
                case 5: return hands->right->thumb;
                case 6: return hands->right->index;
                case 7: return hands->right->middle;
                case 8: return hands->right->ring;
                case 9: return hands->right->pinky;
                default: return Point{100, 100};
                }
            }();

            return {FingerDesc{index}, position};
        }
        iterator& operator++() {
            ++index;
            return *this;
        }
        iterator operator++(int) {
            iterator copy = *this;
            ++index;
            return copy;
        }

        bool operator==(iterator const& other) const {
            return hands == other.hands && index == other.index;
        }
    };

    std::optional<Hand> left;
    std::optional<Hand> right;

    [[nodiscard]] iterator begin() const {
        if (!left.has_value() && right.has_value())
            return {5, this};
        return {0, this};
    }

    [[nodiscard]] iterator end() const {
        if (right.has_value())
            return iterator{10, this};
        else if (left.has_value())
            return iterator{5, this};
        else
            return iterator{0, this};
    }

    FingerRef operator[](FingerDesc finger) const {
        return *std::next(begin(), static_cast<int>(finger));
    }

    [[nodiscard]] std::size_t size() const {
        return (left.has_value() ? 5 : 0) + (right.has_value() ? 5 : 0);
    }
};

static_assert(std::forward_iterator<BothHands::iterator>);
static_assert(std::ranges::viewable_range<BothHands>);
} // namespace finger_tracking

#endif // BOTH_HANDS_HPP
