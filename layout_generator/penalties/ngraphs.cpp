#include "ngraphs.hpp"

#include "range/v3/view/reverse.hpp"
#include "range/v3/view/sliding.hpp"

#include <thread>

namespace finger_tracking {
NGraphSet NGraphSet::computeSetFromKeyPresses(std::span<KeyPress const> keys) {
    NGraphSet result;

    result.digraphOccurrences.reserve(2 * keys.size());
    for (auto digraphs : keys | ranges::views::reverse | ranges::views::sliding(2)) {
        Digraph d;
        std::copy(digraphs.begin(), digraphs.end(), d.keys.begin());
        result.digraphOccurrences.emplace_back(d);
    }

    result.trigraphOccurrences.reserve(2 * keys.size());
    for (auto trigraphs : keys | ranges::views::reverse | ranges::views::sliding(3)) {
        Trigraph t;
        std::copy(trigraphs.begin(), trigraphs.end(), t.keys.begin());
        result.trigraphOccurrences.emplace_back(t);
    }

    // We reverse because quartads are newest key press first and the input is in typing order
    result.quartadOccurrences.reserve(2 * keys.size());
    for (auto quartad : keys | ranges::views::reverse | ranges::views::sliding(4)) {
        Quartad q{};
        std::copy(quartad.begin(), quartad.end(), q.keys.begin());
        result.quartadOccurrences.emplace_back(q);
    }

    return result;
}
} // namespace finger_tracking
