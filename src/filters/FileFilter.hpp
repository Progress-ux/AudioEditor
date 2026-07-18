#include "models/AudioFile.hpp"
#include <string_view>
#include <vector>

class FileFilter
{
private:
    static bool Matches(const AudioFile& file, std::string_view query);
public:
    static std::vector<AudioFile> Apply(const std::vector<AudioFile>& files, std::string_view query);
};
