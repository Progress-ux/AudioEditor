#include "MetadataService.hpp"
#include "exceptions/TagLibException.hpp"

#include <filesystem>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>

AudioFile MetadataService::Load(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
    {
        throw TagLibException("File not found from path: " + path.string());
    }

    auto size = std::filesystem::file_size(path);
    if (size == 0)
    {
        throw TagLibException("File is empty: " + path.string());
    }

    TagLib::FileRef f(path.c_str());
    if (f.isNull() || !f.file() || f.file()->properties().isEmpty())
    {
        throw TagLibException("Cannot open file: " + path.string());
    }

    TagLib::PropertyMap properties = f.file()->properties();

    auto getTag = [&properties](const char* key) -> std::string {
        if (properties.contains(key) && !properties[key].isEmpty())
        {
            return properties[key].front().to8Bit(true);
        }
        return "";
    };

    AudioFile file(path);
    file.title          = getTag("TITLE");
    file.artist         = getTag("ARTIST");
    file.album          = getTag("ALBUM");
    file.comment        = getTag("COMMENT");
    file.genre          = getTag("GENRE");
    file.date           = getTag("DATE");
    file.track_number   = getTag("TRACKNUMBER");

    return file;
}

void MetadataService::Save(const AudioFile& file)
{
    if (!std::filesystem::exists(file.path))
    {
        throw TagLibException("File not found from path: " + file.path.string());
    }

    TagLib::FileRef f(file.path.c_str());
    if (f.isNull())
    {
        throw TagLibException("Cannot open file: " + file.path.string());
    }

    TagLib::PropertyMap properties = f.file()->properties();

    properties.replace("TITLE",       TagLib::String(file.title, TagLib::String::UTF8));
    properties.replace("ARTIST",      TagLib::String(file.artist, TagLib::String::UTF8));
    properties.replace("ALBUM",       TagLib::String(file.album, TagLib::String::UTF8));
    properties.replace("COMMENT",     TagLib::String(file.comment, TagLib::String::UTF8));
    properties.replace("GENRE",       TagLib::String(file.genre, TagLib::String::UTF8));
    properties.replace("DATE",        TagLib::String(file.date, TagLib::String::UTF8));
    properties.replace("TRACKNUMBER", TagLib::String(file.track_number, TagLib::String::UTF8));

    f.file()->setProperties(properties);

    if (!f.save())
    {
        throw TagLibException("Cannot save tags");
    }
}
