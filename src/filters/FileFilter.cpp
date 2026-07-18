#include "FileFilter.hpp"

#include "utils/Utils.hpp"

#include <algorithm>
#include <iterator>
#include <vector>

bool FileFilter::Matches(const AudioFile& file, std::string_view query)
{
    return Utils::ToLower(file.title).find(query) != std::string::npos ||
           Utils::ToLower(file.artist).find(query) != std::string::npos ||
           Utils::ToLower(file.album).find(query) != std::string::npos ||
           Utils::ToLower(file.genre).find(query) != std::string::npos;
}

std::vector<AudioFile> FileFilter::Apply(const std::vector<AudioFile>& files, std::string_view query)
{
    if (query.empty())
        return files;

    std::vector<AudioFile> filteredFiles;
    filteredFiles.reserve(files.size());

    const auto queryLower = Utils::ToLower(query);

    std::copy_if(
        files.begin(), 
        files.end(), 
        std::back_inserter(filteredFiles), 
        [queryLower](const AudioFile& file) 
        {
            return Matches(file, queryLower);
        });

    return filteredFiles;
}
