#include "build_corpus.hpp"

#include <fstream>

namespace finger_tracking {
namespace {
std::string sanitize(std::string file) {
    auto replaceAll = [&](std::string_view from, std::string_view to) {
        size_t start_pos = 0;
        while ((start_pos = file.find(from, start_pos)) != std::string::npos) {
            file.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
    };
    replaceAll("À", "à");
    replaceAll("É", "é");
    replaceAll("Ç", "ç");
    replaceAll("’", "'");
    replaceAll("â", "^a");
    replaceAll("û", "^u");
    replaceAll("ê", "^e");
    replaceAll("ô", "^o");
    replaceAll("î", "^i");
    replaceAll("ï", "¨i");
    replaceAll("œ", "oe");
    replaceAll("«", "\"");
    replaceAll("»", "\"");
    replaceAll("…", "...");
    replaceAll("–", "-");
    replaceAll("}", "");
    replaceAll(")", "");
    replaceAll("]", "");

    return file;
}

std::string readFile(std::filesystem::path path) {
    std::stringstream buffer;
    std::ifstream     file{path.string()};
    buffer << file.rdbuf();
    return sanitize(buffer.str());
}

bool isUsableExtension(std::filesystem::path const& extension) {
    return extension == ".txt" || extension == ".cpp" || extension == ".hpp" || extension == ".py"
        || extension == ".inl" || extension == ".bazel";
}

bool isUsableFolder(std::filesystem::path const& folder) {
    return !is_symlink(folder) && folder.filename() != ".git"
        && !folder.filename().string().starts_with("bazel-");
}

std::string readAllMatchingInDirectory(std::filesystem::path const& directory) {
    std::string total;
    for (auto path : std::filesystem::directory_iterator(directory)) {
        if (is_directory(path.path()) && isUsableFolder(path.path())) {
            total += readAllMatchingInDirectory(path.path());
        } else if (isUsableExtension(path.path().extension())) {
            total += readFile(path);
        }
    }
    return total;
}
} // namespace

std::string buildCorpus(std::vector<std::filesystem::path> const& files) {
    std::string corpus;
    for (auto const& path : files) {
        if (is_directory(path) && isUsableFolder(path))
            corpus += readAllMatchingInDirectory(path);
        else if (isUsableExtension(path.extension()))
            corpus += readFile(path);
    }
    return corpus;
}
} // namespace finger_tracking
