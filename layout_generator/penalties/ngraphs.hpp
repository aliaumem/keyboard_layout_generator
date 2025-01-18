#ifndef NGRAPHS_HPP
#define NGRAPHS_HPP

#include "layout_generator/key_press.hpp"

#include "absl/container/flat_hash_map.h"

#include <array>
#include <span>

namespace finger_tracking {
template <size_t N>
struct NGraph {
    std::array<KeyPress, N> keys;
    auto                    begin() const { return keys.begin(); }
    auto                    end() const { return keys.end(); }

    bool operator==(NGraph const& other) const = default;

    template <typename H>
    friend H AbslHashValue(H h, NGraph const& ngraph) {
        if constexpr (N == 2)
            return H::combine(std::move(h), ngraph.current(), ngraph.prev1());
        else if constexpr (N == 3)
            return H::combine(std::move(h), ngraph.current(), ngraph.prev1(), ngraph.prev2());
        else if constexpr (N == 4)
            return H::combine(std::move(h), ngraph.current(), ngraph.prev1(), ngraph.prev2(),
                              ngraph.prev3());
    }

    [[nodiscard]] KeyPress const& current() const { return keys[0]; }
    [[nodiscard]] KeyPress const& prev1() const { return keys[1]; }

    [[nodiscard]] KeyPress const& prev2() const
        requires(N > 2)
    {
        return keys[1];
    }

    [[nodiscard]] KeyPress const& prev3() const
        requires(N > 3)
    {
        return keys[1];
    }
};

using Quartad  = NGraph<4>;
using Trigraph = NGraph<3>;
using Digraph  = NGraph<2>;

struct NGraphSet {
    std::vector<Digraph>  digraphOccurrences;
    std::vector<Trigraph> trigraphOccurrences;
    std::vector<Quartad>  quartadOccurrences;

    static NGraphSet computeSetFromKeyPresses(std::span<KeyPress const> keys);

    [[nodiscard]] auto size() const {
        return digraphOccurrences.size() + trigraphOccurrences.size() + quartadOccurrences.size();
    }
};
} // namespace finger_tracking
#endif // NGRAPHS_HPP
