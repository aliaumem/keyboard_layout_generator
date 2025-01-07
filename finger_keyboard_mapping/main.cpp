#include "finger_keyboard_mapping/frame.hpp"
#include "finger_keyboard_mapping/from_proto.hpp"

#include <fstream>
#include "finger_keyboard_mapping/finger_to_key_mapping.hpp"

int main(int argc, char const* argv[]) {

    std::ifstream                           file{argv[1], std::ios::binary | std::ios::in};
    finger_tracking::proto::FingerLandmarks landmarks;
    landmarks.ParseFromIstream(&file);

    auto corners = finger_tracking::cast(landmarks.keyboardcorners());
    auto frames  = finger_tracking::cast(landmarks.frames());

    mapFingersToKeys(frames, corners);

    std::cout << landmarks.DebugString() << std::endl;

    return 0;
}
