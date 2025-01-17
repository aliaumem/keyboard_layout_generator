#ifndef PENALIZE_DISTANCE_TO_HOME_ROW_HPP
#define PENALIZE_DISTANCE_TO_HOME_ROW_HPP

#include "layout_generator/penalties/ngraphs.hpp"
#include "layout_generator/row_col.hpp"

namespace finger_tracking::penalties {
inline float penalizeDistanceToHomeRow(KeyPress const& current, KeyPress const& /*previous*/) {
    return static_cast<float>(std::abs(current.row() - Row::Home));
}
} // namespace finger_tracking::penalties
#endif // PENALIZE_DISTANCE_TO_HOME_ROW_HPP
