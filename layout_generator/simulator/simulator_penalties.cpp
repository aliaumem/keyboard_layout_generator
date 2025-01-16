#include "simulator.hpp"

#include "layout_generator/keyboard_layout.hpp"

namespace finger_tracking {
namespace {
float penalizeDistanceToHomeRow(KeyLayoutSequence const& sequence) {
    auto keyRef = sequence.current();
    return 4.f * static_cast<float>(std::abs(keyRef.keyRef.row - static_cast<int>(Row::Home)));
}

float penalizeSameFingerTwice(KeyLayoutSequence const& sequence) {
    auto current  = sequence.current();
    auto previous = sequence.prev1();

    bool isSameFinger = current.fingerDesc() == previous.fingerDesc();

    return current.keyRef != previous.keyRef && isSameFinger ? 4.f : 0.f;
}
} // namespace

Simulator::Simulator(TargetKeyboardLayout const& layout)
    : m_layout{layout}
    , m_penalties{penalizeDistanceToHomeRow, penalizeSameFingerTwice} {}

} // namespace finger_tracking
