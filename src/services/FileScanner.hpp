#pragma once

#include <filesystem>
#include <vector>

class FileScanner
{
public:
    std::vector<std::filesystem::path> Scan(const std::filesystem::path& directory); 
};
