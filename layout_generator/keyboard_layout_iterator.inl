#ifndef KEYBOARD_SHAPE_ITERATOR_HPP
#define KEYBOARD_SHAPE_ITERATOR_HPP

namespace finger_tracking {
template <size_t N>
struct KeyboardLayout<N>::iterator {
    using value_type        = std::pair<Key, Rectangle>;
    using reference         = std::pair<Key, Rectangle>;
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;

    size_t                index = 0;
    KeyboardLayout const* layout;

    reference operator*() const {
        size_t     layer        = index / N;
        auto const indexInLayer = index % N;
        auto const row          = static_cast<Row>(indexInLayer / 12);
        auto const linearCol    = indexInLayer % 12;

        int const split = row == Row::Thumb ? 2 : 6;

        auto const side = linearCol < split ? HandSide::Left : HandSide::Right;
        auto const col  = static_cast<Column>(
            side == HandSide::Left ? split - 1 - linearCol : linearCol - split);

        return layout->keyAt(layer, row, side, col);
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
        return layout == other.layout && index == other.index;
    }
};
} // namespace finger_tracking

#endif // KEYBOARD_SHAPE_ITERATOR_HPP
