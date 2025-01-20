#include "layout_generator/generator/generator.hpp"
#include "layout_generator/azerty_voyager_layout.hpp"

#include "layout_generator/build_corpus.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>
#include "range/v3/view/chunk.hpp"
#include "range/v3/view/transform.hpp"
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
            /*{Key{"LArr"}},
            {Key{"RArr"}},
            {Key{"UArr"}},
            {Key{"DArr"}},
            // Moving through code
            {Key{"LCtl"}, Key{"LArr"}},
            {Key{"LCtl"}, Key{"RArr"}},
            {Key{"LSft"}, Key{"LArr"}},
            {Key{"LSft"}, Key{"RArr"}},
            {Key{"LSft"}, Key{"UArr"}},
            {Key{"LSft"}, Key{"DArr"}},*/
        };

        auto      layout = azertyVoyagerLayout();
        Generator generator{corpus, std::move(shortcuts)};
        auto      resultLayout = generator.run(layout);
        for (auto layer : layout | ranges::views::chunk(52)) {

            for (auto row : layer | ranges::views::chunk(12)) {
                auto keys = row
                          | ranges::views::transform(
                                [&resultLayout](LayoutKeyRef keyRef) -> std::string_view {
                                    return resultLayout.keyAt(keyRef).name;
                                });
                fmt::print("'{}'\n", fmt::join(keys, "' '"));
            }
            fmt::print("\n-----------------\n");
        }
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
