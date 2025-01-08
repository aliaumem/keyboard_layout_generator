#ifndef FROM_PROTO_HPP
#define FROM_PROTO_HPP

#include "finger_keyboard_mapping/frame.hpp"

#include "finger_landmarks/finger_landmarks.pb.h"

namespace finger_tracking {

inline Point cast(proto::Point const& point) {
    return {.x = point.x(), .y = point.y()};
}

inline std::optional<Hand> cast(proto::Hand const& hand) {
    return Hand{
        .thumb  = cast(hand.thumb()),
        .index  = cast(hand.index()),
        .middle = cast(hand.middle()),
        .ring   = cast(hand.ring()),
        .pinky  = cast(hand.pinky()),
    };
}

inline Corners cast(proto::KeyboardCorners const& corners) {
    return {
        .topLeft     = cast(corners.topleft()),
        .topRight    = cast(corners.topright()),
        .bottomRight = cast(corners.bottomright()),
        .bottomLeft  = cast(corners.bottomleft()),
    };
}

inline std::vector<Frame> cast(google::protobuf::RepeatedPtrField<proto::Frame> const& frames) {
    std::vector<Frame> result;
    using optHand = std::optional<Hand>;
    for (auto const& frame : frames) {
        auto timestamp = std::chrono::milliseconds{frame.timestamp()};
        result.emplace_back(
            timestamp, BothHands{
                           .left  = frame.has_lefthand() ? cast(frame.lefthand())
                                                         : static_cast<optHand>(std::nullopt),
                           .right = frame.has_righthand() ? cast(frame.righthand())
                                                          : static_cast<optHand>(std::nullopt),
                       });
    }
    return result;
}
} // namespace finger_tracking

#endif // FROM_PROTO_HPP
