#include "finger_keyboard_mapping/keyboard_timeline_proto.hpp"

#include <fstream>
#include "range/v3/view/adjacent_filter.hpp"
#include "range/v3/view/enumerate.hpp"
#include "range/v3/view/chunk_by.hpp"
#include "finger_keyboard_mapping/keyboard/scancode_key_map.hpp"
#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"

template <>
struct std::formatter<finger_tracking::Key> : std::formatter<std::string_view> {
    template <typename FmtContext>
    auto format(finger_tracking::Key key, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", key.name);
    }
};

using namespace finger_tracking;

struct FingerToKeyMap {
    std::array<std::array<std::uint16_t, 10>, 62> fingerKeyMap{};
    std::uint16_t& operator[](std::pair<std::size_t, FingerDesc> index) {
        return fingerKeyMap[index.first][static_cast<int>(index.second)];
    }
    std::uint16_t const& operator[](std::pair<std::size_t, FingerDesc> index) const {
        return fingerKeyMap[index.first][static_cast<int>(index.second)];
    }
};

FingerToKeyMap computeKeyToFingerAssociation(KeyboardTimeline const& timeline,
                                             ScancodeKeyMap const&   keyMap) {
    FingerToKeyMap fingerToKeyMap{};

    for (auto const& state : timeline) {
        if (state.pressedKeys.empty())
            throw std::runtime_error(
                std::format("No keys pressed at timestamp {}", state.timestamp));
        auto const& [key, finger] = state.pressedKeys.front();
        size_t keyIndex           = keyMap.keyIndex(key);
        fingerToKeyMap[{keyIndex, finger}]++;
    }

    return fingerToKeyMap;
}

int main(int argc, char* argv[]) {
    std::cout << "Starting computation" << std::endl;
    try {
        proto::KeyboardTimeline protoTimeline;
        std::ifstream           protoFile(argv[1], std::ios::binary | std::ios::in);
        if (!protoFile.is_open())
            throw std::runtime_error(std::format("Failed to open {}", argv[1]));
        protoTimeline.ParseFromIstream(&protoFile);
        auto timeline = cast(protoTimeline);

        ScancodeKeyMap scancodeKeyMap{};
        {
            // finger per key
            auto association = computeKeyToFingerAssociation(timeline, scancodeKeyMap);

            for (auto const& [i, fingers] : ranges::views::enumerate(association.fingerKeyMap)) {
                std::cout << scancodeKeyMap.keys[i].name << "\t";
                for (auto const& [fingerInt, finger] : ranges::views::enumerate(fingers)) {
                    if (finger != 0) {
                        FingerDesc desc{static_cast<int>(fingerInt)};
                        std::cout << std::format("[{}: {}]", desc, finger);
                    }
                }
                std::cout << std::endl;
            }
        }
        {
            std::chrono::milliseconds lastTimestamp{};
            auto chunked
                = timeline | ranges::views::chunk_by([](auto const& prev, auto const& next) {
                      using namespace std::chrono_literals;
                      return next.timestamp - prev.timestamp < 200ms;
                  });
            for (auto&& chunk : chunked) {
                std::cout << "-----------" << std::endl;
                for (auto const& state : chunk) {
                    auto delta = state.timestamp - lastTimestamp;
                    lastTimestamp = state.timestamp;
                    std::cout << delta << "\t"
                              << std::format("{} {}", state.pressedKeys.front().first,
                                             state.pressedKeys.front().second)
                              << std::endl;
                }
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

    std::cout << "Done" << std::endl;

    return 0;
}
