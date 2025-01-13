#include "finger_keyboard_mapping/keyboard/scancode_key_map.hpp"

#include "finger_keyboard_mapping/mapping/finger_to_key_mapping.hpp"
#include "finger_keyboard_mapping/mapping/closest_finger_mapping.hpp"
#include "finger_keyboard_mapping/mapping/map_keylog_timeline.hpp"

#include "finger_keyboard_mapping/from_proto.hpp"
#include "finger_keyboard_mapping/keyboard_timeline_proto.hpp"
#include <fstream>
#include <ranges>

using namespace std::string_literals;

finger_tracking::KeyboardTimeline computeTimelineForOffset(
    std::optional<std::chrono::milliseconds>&     timeOffset,
    std::vector<finger_tracking::Frame> const&    frames,
    std::vector<finger_tracking::KeyEvent> const& keyEvents,
    finger_tracking::KeyboardShape const&         shape,
    finger_tracking::ScancodeKeyMap const&        scancodeKeyMap) {

    finger_tracking::FingerToKeyMapper mapper{shape, scancodeKeyMap};

    if (timeOffset.has_value())
        return mapper.mapFingersToKeys(frames, keyEvents, *timeOffset);

    std::vector<finger_tracking::KeyboardTimeline> timelines{};
    for (auto i = 0; i < 500; ++i) {
        std::chrono::milliseconds delta{i * 1};

        timelines.push_back(mapper.mapFingersToKeys(frames, keyEvents, delta));
    }

    std::ranges::sort(timelines, [](auto const& lhs, auto const& rhs) {
        return lhs.totalDistance() < rhs.totalDistance();
    });

    auto it = std::ranges::max_element(timelines, [](auto const& lhs, auto const& rhs) {
        return lhs.totalPressedKeyFrameCount() < rhs.totalPressedKeyFrameCount();
    });
    timeOffset.emplace(std::chrono::milliseconds{std::distance(timelines.begin(), it)});
    return std::move(*it);
}

int main(int argc, char const* argv[]) {

    std::string                              landmarksPath = argv[1];
    std::optional<std::chrono::milliseconds> timeOffset    = std::nullopt;
    if (argc > 2)
        timeOffset = std::chrono::milliseconds{std::atoi(argv[2])};

    auto        basePath   = landmarksPath.substr(0, landmarksPath.rfind("_landmarks.binarypb"));
    std::string keylogPath = basePath + "_keylog.binarypb"s;

    finger_tracking::proto::FingerLandmarks landmarks;
    {
        std::ifstream file{landmarksPath, std::ios::binary | std::ios::in};
        landmarks.ParseFromIstream(&file);
    }

    keylog::proto::KeyLog keylog;
    {
        std::ifstream file(keylogPath, std::ios::binary | std::ios::in);
        keylog.ParseFromIstream(&file);
    }

    finger_tracking::ScancodeKeyMap scancodeKeyMap;

    auto frames = finger_tracking::cast(landmarks.frames());
    auto shape  = finger_tracking::KeyboardShape::defaultShape();

    auto keyEventsRng = keylog.keyevents() | std::views::transform(finger_tracking::toKeyEvent);
    std::vector<finger_tracking::KeyEvent> keyEvents{keyEventsRng.begin(), keyEventsRng.end()};

    {
        auto timeline
            = computeTimelineForOffset(timeOffset, frames, keyEvents, shape, scancodeKeyMap);
        auto          protoTimeline = cast(timeline);
        std::ofstream file{basePath + "_timeline.binarypb", std::ios::binary | std::ios::out};
        protoTimeline.SerializeToOstream(&file);
    }

    std::cout << "offset: " << (*timeOffset) << std::endl;
    {
        auto keylogTimeline = mapForStats(scancodeKeyMap, frames, shape, keyEvents, *timeOffset);
        auto protoTimeline  = cast(keylogTimeline);
        std::ofstream file{basePath + "_keylog_timeline.binarypb",
                           std::ios::binary | std::ios::out};
        protoTimeline.SerializeToOstream(&file);
        // std::cout << protoTimeline.DebugString();
    }

    return 0;
}
