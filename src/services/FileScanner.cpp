#include "FileScanner.hpp"

std::vector<std::filesystem::path> FileScanner::Scan(const std::filesystem::path& directory)
{
    std::vector<std::filesystem::path> files;
    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        std::string filename = entry.path().filename();
        if (filename.ends_with(".mp3"))
        {
            files.push_back(entry.path());
        }
    }
    return files;
}
