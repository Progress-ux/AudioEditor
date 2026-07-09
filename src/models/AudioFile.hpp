#pragma once

#include <filesystem>

struct AudioFile
{
    AudioFile() = default;

    explicit AudioFile(std::filesystem::path p)
        : path(std::move(p)) {}

    std::filesystem::path path;
    std::string title;
    std::string artist;
    std::string album;
    std::string comment;
    std::string genre;
    std::string date;
    std::string track_number;
};
