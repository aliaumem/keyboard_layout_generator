#include "finger_keyboard_mapping/keyboard/scancode_key_map.hpp"
#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"

#include "finger_keyboard_mapping/mapping/finger_to_key_mapping.hpp"
#include "finger_keyboard_mapping/mapping/closest_finger_mapping.hpp"

#include "finger_keyboard_mapping/from_proto.hpp"
#include <fstream>
#include <ranges>

using namespace std::string_literals;

void mapForStats(finger_tracking::ScancodeKeyMap const&        scancodeKeyMap,
                 std::vector<finger_tracking::Frame> const&    frames,
                 finger_tracking::KeyboardShape const&         shape,
                 std::vector<finger_tracking::KeyEvent> const& keyEvents,
                 std::chrono::milliseconds                     timeOffset) {

    for (auto const& [timestamp, isPressed, code] : keyEvents) {

        if (!isPressed)
            continue;

        using std::chrono::milliseconds;
        auto prevFrame = std::lower_bound(frames.begin(), frames.end(), timestamp - timeOffset,
                                          [](finger_tracking::Frame const& frame, milliseconds t) {
                                              return frame.timestamp < t;
                                          });
        if ((timestamp - timeOffset) - prevFrame->timestamp > milliseconds{16})
            ++prevFrame;

        if (prevFrame == frames.end())
            break;

        auto key         = scancodeKeyMap.scanCodeToKey(code);
        auto maybeFinger = closestFinger(shape, key, prevFrame->hands);

        std::cout << timestamp << "\t" << (isPressed ? "v " : "^ ") << std::hex << code.scancode
                  << std::dec << "  " << key.name << "\t"
                  << (maybeFinger.has_value() ? std::format("{}", *maybeFinger) : "None") << "\n";
    }
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

    std::cout << "frames: " << frames.back().timestamp << "\tkeys: "
              << std::chrono::milliseconds(keylog.keyevents().rbegin()->timestamp_ms())
              << std::endl;

    auto shape = finger_tracking::KeyboardShape::defaultShape();

    auto keyEventsRng = keylog.keyevents() | std::views::transform(finger_tracking::toKeyEvent);
    std::vector<finger_tracking::KeyEvent> keyEvents{keyEventsRng.begin(), keyEventsRng.end()};

    finger_tracking::FingerToKeyMapper mapper{shape, scancodeKeyMap};

    auto timeline = [&] {
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
    }();

    // auto it = std::ranges::max_element(timelines, std::less{}, [](auto const& timeline) -> long {
    //     return static_cast<long>(timeline.eventCount());
    //     //- static_cast<long>(timeline.totalDistance() / timeline.eventCount());
    // });

    // auto timeOffset = std::chrono::milliseconds{427};

    std::cout << "offset: " << (*timeOffset) << std::endl;

    auto protoTimeline = cast(timeline);
    // std::cout << protoTimeline.DebugString();

    {
        std::ofstream file{basePath + "_timeline.binarypb", std::ios::binary | std::ios::out};
        protoTimeline.SerializeToOstream(&file);
    }

    mapForStats(scancodeKeyMap, frames, shape, keyEvents, *timeOffset);

    return 0;
}
