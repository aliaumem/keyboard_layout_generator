#include "layout_generator/penalties/ngraphs.hpp"

#include "catch2/catch_test_macros.hpp"
#include "layout_generator/row_col.hpp"

using namespace finger_tracking;

SCENARIO("Digraph Trigraphs and Quartads can be computed from a list of key presses") {
    std::vector presses = {
        KeyPress{
            {0, HandSide::Left, Row::Home, Column::Middle},
            Finger::Middle,
            Point16{30, 52},
            true,
        },
        KeyPress{

            {0, HandSide::Right, Row::Home, Column::Middle},
            Finger::Middle,
            Point16{153, 52},
            true,
        },
        KeyPress{

            {1, HandSide::Left, Row::Top, Column::Pinky},
            Finger::Pinky,
            Point16{8, 5},
            true,
        },
        KeyPress{

            {1, HandSide::Right, Row::Home, Column::Middle},
            Finger::Middle,
            Point16{153, 52},
            false,
        },
        KeyPress{

            {1, HandSide::Left, Row::Thumb, Column::IndexExt},
            Finger::Thumb,
            Point16{70, 152},
            true,
        },
    };

    NGraphSet set = NGraphSet::computeSetFromKeyPresses(presses);

    CHECK(set.digraphOccurrences.size() == 4);
    CHECK(set.trigraphOccurrences.size() == 3);
    CHECK(set.quartadOccurrences.size() == 2);
    REQUIRE(set.size() == 9);
}
