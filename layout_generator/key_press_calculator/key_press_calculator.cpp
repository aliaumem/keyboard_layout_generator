#include "key_press_calculator.hpp"

#include "layout_generator/to_shifted_keys.hpp"

#include "layout_generator/keyboard_layout.hpp"
#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"

#include <range/v3/view/chunk_by.hpp>
#include "range/v3/view/reverse.hpp"

#include <algorithm>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/algorithm/lower_bound.hpp>

#include <fmt/format.h>

namespace finger_tracking {
KeyPressCalculator::KeyPressCalculator(TargetKeyboardLayout const& layout)
    : m_layout{layout} {
    m_reverseLookup.reserve(m_layout.size());
    for (auto const& keyRef : m_layout) {
        if (auto key = m_layout.keyAt(keyRef); key.isValid()) {
            m_reverseLookup.try_emplace(key, keyRef);
            if (auto shiftKey = toShiftedKey(key); shiftKey.isValid())
                m_reverseLookup.try_emplace(shiftKey, keyRef.withShift());
            if (auto altKey = toAltKey(key); altKey.isValid())
                m_reverseLookup.try_emplace(altKey, keyRef.withAlt());
        }
        if (auto key = m_layout.keyAt(keyRef.withHeld()); key.isValid())
            m_reverseLookup.try_emplace(key, keyRef);
    }
}

void KeyPressCalculator::emplaceKeyRefInSequence(KeyLayoutSequence& sequence, LayoutKeyRef keyRef,
                                                 bool isPress) const {
    sequence.emplace_back(keyRef, m_layout.fingerFor(keyRef), isPress);
}

void KeyPressCalculator::insertLayoutChangeSequence(LayerId fromLayer, KeyLayoutSequence& sequence,
                                                    LayoutKeyRef keyRef) const {
    if (fromLayer != LayerId::defaultLayer) {
        auto extraKey    = m_layout.layerTransitionKey(fromLayer);
        auto extraKeyRef = lookupKey(extraKey);
        emplaceKeyRefInSequence(sequence, extraKeyRef, false);
    }

    if (keyRef.layer != LayerId::defaultLayer) {
        auto extraKey    = m_layout.layerTransitionKey(keyRef.layer);
        auto extraKeyRef = lookupKey(extraKey);
        emplaceKeyRefInSequence(sequence, extraKeyRef, true);
    }
}

KeyLayoutSequence KeyPressCalculator::sequenceForKey(LayerId& fromLayer, Key const& key) const {
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
    LayerId               layer = LayerId::defaultLayer;
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
        LayerId layer = LayerId::defaultLayer;
        for (auto const& key : shortcut) {
            auto seq = sequenceForKey(layer, key);
            std::copy(seq.begin(), seq.end(), std::back_inserter(result));
        }
    }
    return result;
}

auto KeyPressCalculator::lookupKey(Key const& key) const -> LayoutKeyRef {

    auto it = m_reverseLookup.find(key);
    if (it == m_reverseLookup.end())
        throw std::invalid_argument{fmt::format("Key '{}' does not exist in layout", key)};

    return it->second;
}
} // namespace finger_tracking
