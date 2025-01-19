#include "key_press_calculator.hpp"

#include "layout_generator/keyboard_layout.hpp"
#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"

#include <range/v3/view/chunk_by.hpp>
#include "range/v3/view/reverse.hpp"

#include <algorithm>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/algorithm/lower_bound.hpp>
#include <numeric>
#include <fmt/format.h>

namespace finger_tracking {
void KeyPressCalculator::emplaceKeyRefInSequence(KeyLayoutSequence& sequence, LayoutKeyRef keyRef,
                                                 bool isPress) const {
    sequence.emplace_back(keyRef, m_layout.fingerFor(keyRef), isPress);
}

void KeyPressCalculator::insertLayoutChangeSequence(
    std::uint8_t fromLayer, KeyLayoutSequence& sequence, LayoutKeyRef keyRef) const {
    if (fromLayer != 0) {
        auto extraKey    = m_layout.layerTransitionKey(fromLayer);
        auto extraKeyRef = lookupKey(extraKey);
        emplaceKeyRefInSequence(sequence, extraKeyRef, false);
    }

    if (keyRef.layer != 0) {
        auto extraKey    = m_layout.layerTransitionKey(keyRef.layer);
        auto extraKeyRef = lookupKey(extraKey);
        emplaceKeyRefInSequence(sequence, extraKeyRef, true);
    }
}

KeyPressCalculator::KeyPressCalculator(TargetKeyboardLayout const& layout)
    : m_layout{layout} {
    m_reverseLookup.reserve(m_layout.size());
    for (auto const& [index, keyRef] : m_layout | ranges::views::enumerate) {
        if (auto key = m_layout.keyAt(keyRef); key.isValid())
            m_reverseLookup.emplace_back(key, index, false);
        if (auto key = m_layout.heldKeyAt(keyRef); key.isValid())
            m_reverseLookup.emplace_back(key, index, true);
    }

    ranges::actions::sort(m_reverseLookup, std::less{}, [](auto const& lookupInfo) {
        return lookupInfo.key;
    });
}

KeyLayoutSequence KeyPressCalculator::sequenceForKey(std::uint8_t& fromLayer,
                                                     Key const&    key) const {
    KeyLayoutSequence sequence;

    auto keyRef = lookupKey(key);

    if (keyRef.layer != fromLayer) {
        insertLayoutChangeSequence(fromLayer, sequence, keyRef);
        fromLayer = keyRef.layer;
    }

    emplaceKeyRefInSequence(sequence, keyRef);

    return sequence;
}

auto belongInTheSameUTF8Char(char, char c) -> bool {
    char const singleByte     = 0b0000'0000;
    char const singleByteMask = 0b1000'0000;

    char const twoBytes     = 0b1100'0000;
    char const twoBytesMask = 0b1110'0000;

    char const threeBytes     = 0b1110'0000;
    char const threeBytesMask = 0b1111'0000;

    char const fourBytes     = 0b1111'0000;
    char const fourBytesMask = 0b1111'1000;

    bool isSingleChar     = (c & singleByteMask) == singleByte;
    bool isTwoBytesChar   = (c & twoBytesMask) == twoBytes;
    bool isThreeBytesChar = (c & threeBytesMask) == threeBytes;
    bool isFourBytesChar  = (c & fourBytesMask) == fourBytes;
    return !(isSingleChar || isTwoBytesChar || isThreeBytesChar || isFourBytesChar);
}

std::vector<KeyPress> KeyPressCalculator::simulate(std::string_view corpus) const {
    std::uint8_t          layer = 0;
    std::vector<KeyPress> result;
    result.reserve(corpus.size() * 5 / 4); // 20% extra for layer changes
    for (auto chunk : corpus | ranges::views::chunk_by(belongInTheSameUTF8Char)) {
        std::string_view keyStr{
            &*chunk.begin(), static_cast<size_t>(ranges::distance(chunk.begin(), chunk.end()))};
        auto seq = sequenceForKey(layer, Key{keyStr});
        std::copy(seq.begin(), seq.end(), std::back_inserter(result));
    }

    return result;
}

std::vector<KeyPress> KeyPressCalculator::simulateShortcuts(
    std::vector<static_vector<Key, 4>> const& shortcuts) const {
    std::vector<KeyPress> result;
    result.reserve(shortcuts.size() * 3);
    for (auto const& shortcut : shortcuts) {
        std::uint8_t layer = 0;
        for (auto const& key : shortcut) {
            auto seq = sequenceForKey(layer, key);
            std::copy(seq.begin(), seq.end(), std::back_inserter(result));
        }
    }
    return result;
}

auto KeyPressCalculator::lookupKey(Key const& key) const -> LayoutKeyRef {
    auto it = ranges::lower_bound(m_reverseLookup, key, std::less{}, [](auto const& lookupInfo) {
        return lookupInfo.key;
    });
    if (it == m_reverseLookup.end() || it->key != key)
        throw std::invalid_argument{fmt::format("Key '{}' does not exist in layout", key)};
    auto keyRef = m_layout.toKeyRef(m_layout.begin() + it->index);

    return keyRef;
}
} // namespace finger_tracking
