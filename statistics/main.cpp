#include "statistics/finger_key_frequency/finger_to_key_association.hpp"
#include "statistics/key_sequence_printer.hpp"

#include "range/v3/view/adjacent_filter.hpp"
#include "range/v3/view/chunk_by.hpp"

#include "finger_keyboard_mapping/keyboard_timeline_proto.hpp"

#include <fstream>

using namespace finger_tracking;
using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
    try {
        proto::KeyboardTimeline protoTimeline;
        std::ifstream           protoFile(argv[1], std::ios::binary | std::ios::in);
        if (!protoFile.is_open())
            throw std::runtime_error(fmt::format("Failed to open {}", argv[1]));
        protoTimeline.ParseFromIstream(&protoFile);

        // We need to keep the proto around because this timeline has references on the key names
        auto timeline = cast(protoTimeline);

        {
            // finger per key
            FingerToKeyAssociation fingerPerKey{};
            auto association = fingerPerKey.computeKeyToFingerAssociation(timeline);
            fingerPerKey.print(association);
        }
        {
            KeySequencePrinter printer{};
            auto               chunked
                = timeline | ranges::views::chunk_by([](auto const& prev, auto const& next) {
                      return next.timestamp - prev.timestamp < 300ms;
                  });
            for (auto&& chunk : chunked) {
                printer.print(chunk);
            }
        }
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    // build digraphs/trigraphs ?
    //  - which finger sequence?
    //  - which geometric sequence per finger?
    //  - how much time between digraphs?/trigraphs?
    // measure finger freq / key
    // measure delta per what???

    return 0;
}
