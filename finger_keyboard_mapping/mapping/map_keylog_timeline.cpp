#include "map_keylog_timeline.hpp"

#include "finger_keyboard_mapping/mapping/closest_finger_mapping.hpp"

namespace finger_tracking {
KeyboardTimeline mapForStats(ScancodeKeyMap const& scancodeKeyMap, std::vector<Frame> const& frames,
                             KeyboardShape const& shape, std::vector<KeyEvent> const& keyEvents,
                             std::chrono::milliseconds timeOffset) {
    using std::chrono::milliseconds;

    KeyboardTimeline::Builder builder;
    auto compareTimestamps = [](Frame const& frame, milliseconds t) { return frame.timestamp < t; };

    for (auto const& [timestamp, isPressed, code] : keyEvents) {
        builder.nextFrame(timestamp);

        auto const key               = scancodeKeyMap.scanCodeToKey(code);
        auto       adjustedTimestamp = timestamp - timeOffset;

        if (!isPressed) {
            builder.released(key);
            continue;
        }

        auto prevFrame
            = std::lower_bound(frames.begin(), frames.end(), adjustedTimestamp, compareTimestamps);

        // We're already out of landmarks, so no need to keep trying to match anything
        if (prevFrame == frames.end())
            break;

        if (auto nextFrame = ++prevFrame; nextFrame != frames.end()) {
            bool closestToNextFrame = adjustedTimestamp - prevFrame->timestamp
                                    > (nextFrame->timestamp - prevFrame->timestamp) / 2;
            if (closestToNextFrame)
                prevFrame = nextFrame;
        }

        if (auto maybeFinger = closestFinger(shape, key, prevFrame->hands); maybeFinger.has_value())
            builder.pressed(key, maybeFinger->fingerDesc);

        // std::cout << timestamp << "\t" << (isPressed ? "v " : "^ ") << std::hex << code.scancode
        //           << std::dec << "  " << key.name << "\t"
        //           << (maybeFinger.has_value() ? std::format("{}", *maybeFinger) : "None") <<
        //           "\n";
    }

    return builder.build();
}
} // namespace finger_tracking
