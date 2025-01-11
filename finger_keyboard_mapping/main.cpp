#include "finger_keyboard_mapping/frame.hpp"
#include "finger_keyboard_mapping/from_proto.hpp"
#include "finger_keyboard_mapping/finger_to_key_mapping.hpp"
#include "finger_keyboard_mapping/scancode_key_map.hpp"
#include "finger_keyboard_mapping/finger_print_helpers.hpp"

#include <finger_keyboard_mapping/keyboard_shape.pb.h>
#include <keylogger/keylog.pb.h>
#include <fstream>
#include <ranges>

using namespace std::string_literals;

void mapForStats(finger_tracking::ScancodeKeyMap const&        scancodeKeyMap,
                 std::vector<finger_tracking::Frame> const&    frames,
                 finger_tracking::KeyboardShape const&         shape,
                 std::vector<finger_tracking::KeyEvent> const& keyEvents) {

    for (auto const& [timestamp, isPressed, code] : keyEvents) {
        // if (!isPressed)
        //     continue;

        using std::chrono::milliseconds;
        auto prevFrame = std::lower_bound(
            frames.begin(), frames.end(), timestamp,
            [](finger_tracking::Frame const& frame, milliseconds timestamp) {
                return frame.timestamp < timestamp;
            });

        if (prevFrame == frames.end())
            break;

        auto key         = scancodeKeyMap.scanCodeToKey(code);
        auto maybeFinger = shape.closestFinger(key, prevFrame->hands);

        std::cout << timestamp << "\t" << (isPressed ? "v " : "^ ") << std::hex << code.scancode
                  << std::dec << "  " << key.name << "\t"
                  << (maybeFinger.has_value() ? std::format("{}", *maybeFinger) : "None") << "\n";
    }
}
int main(int argc, char const* argv[]) {

    std::string landmarksPath = argv[1];

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

    auto timeline = mapFingersToKeys(frames, keyEvents, shape, scancodeKeyMap);

    auto protoTimeline = cast(timeline);
    std::cout << protoTimeline.DebugString();

    {
        std::ofstream file{basePath + "_timeline.binarypb", std::ios::binary | std::ios::out};
        protoTimeline.SerializeToOstream(&file);
    }

    mapForStats(scancodeKeyMap, frames, shape, keyEvents);

    return 0;
}
