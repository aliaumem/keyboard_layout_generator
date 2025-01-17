#include "simulator.hpp"

#include "layout_generator/keyboard_layout.hpp"
#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"

#include <range/v3/view/chunk_by.hpp>
#include "range/v3/view/reverse.hpp"

#include <numeric>
#include <format>

namespace finger_tracking {
void Simulator::emplaceKeyRefInSequence(KeyLayoutSequence& sequence, LayoutKeyRef keyRef,
                                        bool isPress) const {
    sequence.emplace_back(keyRef, m_layout.fingerFor(keyRef), isPress,
                          m_layout.keyAt(keyRef).second.center().cast<std::int16_t>());
}

void Simulator::insertLayoutChangeSequence(std::uint8_t fromLayer, KeyLayoutSequence& sequence,
                                           LayoutKeyRef keyRef) const {
    if (fromLayer != 0) {
        auto extraKeyRef = m_layout.layerTransitionKey(fromLayer);
        emplaceKeyRefInSequence(sequence, extraKeyRef, false);
    }
    if (keyRef.layer != 0) {
        auto extraKeyRef = m_layout.layerTransitionKey(keyRef.layer);
        emplaceKeyRefInSequence(sequence, extraKeyRef, true);
    }
}

KeyLayoutSequence Simulator::sequenceForKey(std::uint8_t fromLayer, Key const& key) const {
    KeyLayoutSequence sequence;

    auto keysMatch = [&key](auto const& pairs) -> bool { return pairs.first == key; };
    auto it        = std::ranges::find_if(m_layout, keysMatch);
    if (it == m_layout.end())
        throw std::invalid_argument{std::format("Key '{}' does not exist in layout", key)};

    auto keyRef = m_layout.toKeyRef(it);

    if (keyRef.layer != fromLayer)
        insertLayoutChangeSequence(fromLayer, sequence, keyRef);

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

std::vector<KeyPress> Simulator::simulate(std::string_view corpus) const {
    std::uint8_t          layer = 0;
    std::vector<KeyPress> result;
    result.reserve(corpus.size() * 5 / 4); // 20% extra for layer changes
    for (auto chunk : corpus | ranges::views::chunk_by(belongInTheSameUTF8Char)) {
        std::string_view keyStr{
            &*chunk.begin(), static_cast<size_t>(ranges::distance(chunk.begin(), chunk.end()))};
        auto seq = sequenceForKey(layer, Key{keyStr});
        std::copy(seq.begin(), seq.end(), std::back_inserter(result));
        layer = seq.back().keyRef.layer;
    }

    return result;
}
} // namespace finger_tracking
