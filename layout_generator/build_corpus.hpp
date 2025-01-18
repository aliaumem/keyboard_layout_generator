#ifndef BUILD_CORPUS_HPP
#define BUILD_CORPUS_HPP

#include <filesystem>
#include <string>
#include <vector>

namespace finger_tracking {
std::string buildCorpus(std::vector<std::filesystem::path> const& files);
}

#endif // BUILD_CORPUS_HPP
