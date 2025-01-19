#ifndef KEYBOARD_SHAPE_ITERATOR_HPP
#define KEYBOARD_SHAPE_ITERATOR_HPP

namespace finger_tracking {
template <size_t N>
struct KeyboardLayout<N>::iterator {
    using value_type        = LayoutKeyRef;
    using reference         = LayoutKeyRef;
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;

    iterator() = default;

    iterator(size_t index, KeyboardLayout const* layout)
        : index(index)
        , layout(layout) {}

    size_t                index = 0;
    KeyboardLayout const* layout;

    reference operator*() const { return layout->toKeyRef(*this); }

    iterator& operator++() {
        ++index;
        return *this;
    }

    iterator operator++(int) {
        iterator copy = *this;
        ++index;
        return copy;
    }

    iterator operator+(difference_type offset) const {
        iterator copy = *this;
        copy.index += offset;
        return copy;
    }

    bool operator==(iterator const& other) const {
        return layout == other.layout && index == other.index;
    }
};
} // namespace finger_tracking

#endif // KEYBOARD_SHAPE_ITERATOR_HPP
