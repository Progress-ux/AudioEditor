#pragma once

#include <filesystem>

#include "models/AudioFile.hpp"

class MetadataService
{
public:
    AudioFile Load(const std::filesystem::path& path);

    void Save(const AudioFile& file);
};
