#include "finger_keyboard_mapping/frame.hpp"
#include "finger_keyboard_mapping/from_proto.hpp"
#include "finger_keyboard_mapping/finger_to_key_mapping.hpp"

#include <fstream>
#include <finger_keyboard_mapping/keyboard_shape.pb.h>

int main(int argc, char const* argv[]) {

    std::ifstream                           file{argv[1], std::ios::binary | std::ios::in};
    finger_tracking::proto::FingerLandmarks landmarks;
    landmarks.ParseFromIstream(&file);

    auto corners = finger_tracking::cast(landmarks.keyboardcorners());
    auto frames  = finger_tracking::cast(landmarks.frames());

    mapFingersToKeys(frames);

    finger_tracking::Rectangle rect = finger_tracking::KeyboardShape::defaultShape().aabb();
    std::cout << rect.topLeft.x << ", " << rect.topLeft.y << "  " << rect.size.width << ", "
              << rect.size.height << std::endl;

    return 0;
}
