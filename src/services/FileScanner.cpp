#include "FileScanner.hpp"
#include <filesystem>
#include <system_error>

std::vector<std::filesystem::path> FileScanner::Scan(const std::filesystem::path& directory)
{
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory))
    {
        throw std::filesystem::filesystem_error(
            "Directory not found",
            directory,
            std::make_error_code(std::errc::no_such_file_or_directory)
        );
    }

    std::vector<std::filesystem::path> files;
    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.path().extension() == ".mp3")
        {
            files.push_back(entry.path());
        }
    }
    return files;
}
