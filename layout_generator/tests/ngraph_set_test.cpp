#include "layout_generator/penalties/ngraphs.hpp"

#include "catch2/catch_test_macros.hpp"
#include "layout_generator/row_col.hpp"

using namespace finger_tracking;

SCENARIO("Digraph Trigraphs and Quartads can be computed from a list of key presses") {
    std::vector<KeyPress> presses = {
        {
            .keyRef
            = {.layer = 0, .side = HandSide::Left, .row = Row::Home, .column = Column::Middle},
            .finger   = Finger::Middle,
            .isPress  = true,
            .position = Point16{30, 52},
        },
        {
            .keyRef
            = {.layer = 0, .side = HandSide::Right, .row = Row::Home, .column = Column::Middle},
            .finger   = Finger::Middle,
            .isPress  = true,
            .position = Point16{153, 52},
        },
        {
            .keyRef
            = {.layer = 1, .side = HandSide::Left, .row = Row::Top, .column = Column::Pinky},
            .finger   = Finger::Pinky,
            .isPress  = true,
            .position = Point16{8, 5},
        },
        {
            .keyRef
            = {.layer = 1, .side = HandSide::Right, .row = Row::Home, .column = Column::Middle},
            .finger   = Finger::Middle,
            .isPress  = false,
            .position = Point16{153, 52},
        },
        {
            .keyRef
            = {.layer = 1, .side = HandSide::Left, .row = Row::Thumb, .column = Column::IndexExt},
            .finger   = Finger::Thumb,
            .isPress  = true,
            .position = Point16{70, 152},
        },
    };

    NGraphSet set = NGraphSet::computeSetFromKeyPresses(presses);

    CHECK(set.digraphOccurrences.size() == 4);
    CHECK(set.trigraphOccurrences.size() == 3);
    CHECK(set.quartadOccurrences.size() == 2);
    REQUIRE(set.size() == 9);
}
