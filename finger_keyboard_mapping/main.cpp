#include "finger_keyboard_mapping/frame.hpp"
#include "finger_keyboard_mapping/from_proto.hpp"
#include "finger_keyboard_mapping/finger_to_key_mapping.hpp"
#include "finger_keyboard_mapping/scancode_key_map.hpp"

#include <finger_keyboard_mapping/keyboard_shape.pb.h>
#include <keylogger/keylog.pb.h>
#include <fstream>
#include <ranges>

using namespace std::string_literals;

int main(int argc, char const* argv[]) {

    std::string landmarksPath = argv[1];

    std::string keylogPath
        = landmarksPath.substr(0, landmarksPath.rfind("_landmarks.binarypb")) + "_keylog.binarypb"s;

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

    mapFingersToKeys(frames);

    auto shape = finger_tracking::KeyboardShape::defaultShape();

    for (auto const& evt :
         keylog.keyevents() | std::views::transform(finger_tracking::toKeyEvent)) {

        finger_tracking::Key key = scancodeKeyMap.scanCodeToKey(evt.code);

        auto it = std::find(shape.keys().begin(), shape.keys().end(), key);

        using namespace finger_tracking::geo_literals;
        finger_tracking::Point pt = it != shape.keys().end() ? it->aabb.center() : (-1_x, -1_y);

        std::cout << (evt.isPressed ? 'v' : '^') << ' ' << (evt.code.isE0 ? "E0 " : "")
                  << (evt.code.isE1 ? "E1 " : "") << std::hex << evt.code.scancode << std::dec
                  << "\t" << key.name << "\t" << pt.x << ',' << pt.y << std::dec << std::endl;
    }

    return 0;
}
