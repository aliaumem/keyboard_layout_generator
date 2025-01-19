#include "layout_generator/generator/generator.hpp"
#include "layout_generator/azerty_voyager_layout.hpp"

#include "layout_generator/build_corpus.hpp"

#include <fmt/format.h>
#include <iostream>

using namespace finger_tracking;

int main(int argc, char* argv[]) {

    // - Compute the key sequence for the quartad
    // - Compute registered penalties
    // - Fill in penalties
    // - Mutate layouts
    // - Do the annealing

    try {
        std::vector<std::filesystem::path> files;
        for (int i = 1; i < argc; ++i) {
            files.emplace_back(argv[i]);
        }
        std::string corpus = buildCorpus(files);

        std::vector<static_vector<Key, 4>> shortcuts{
            {Key{"LCtl"}, Key{"Tab"}},
            {Key{"LCtl"}, Key{"LSft"}, Key{"Tab"}},
            {Key{"LAlt"}, Key{"Tab"}},
            {Key{"LAlt"}, Key{"LSft"}, Key{"Tab"}},
            {Key{"LCtl"}, Key{"X"}},
            {Key{"LCtl"}, Key{"C"}},
            {Key{"LCtl"}, Key{"V"}},
            {Key{"LCtl"}, Key{"Z"}},
            {Key{"LCtl"}, Key{"LSft"}, Key{"Z"}},
            // Directional arrows
            {Key{"LArr"}},
            {Key{"RArr"}},
            {Key{"UArr"}},
            {Key{"DArr"}},
            // Moving through code
            {Key{"LCtl"}, Key{"LArr"}},
            {Key{"LCtl"}, Key{"RArr"}},
            {Key{"LSft"}, Key{"LArr"}},
            {Key{"LSft"}, Key{"RArr"}},
            {Key{"LSft"}, Key{"UArr"}},
            {Key{"LSft"}, Key{"DArr"}},
        };

        auto      layout = azertyVoyagerLayout();
        Generator generator{};
        generator.computeLayoutPenalty(corpus, shortcuts, layout);

    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
