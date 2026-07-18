#include "models/AudioFile.hpp"
#include "filters/FileFilter.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("Filter: filtering audio files", "[filter]")
{
    std::vector<AudioFile> files {
        AudioFile{ "tracks/01.mp3" },
        AudioFile{ "tracks/02.mp3" },
        AudioFile{ "tracks/03.mp3" },
        AudioFile{ "tracks/04.mp3" },
        AudioFile{ "tracks/05.mp3" },
        AudioFile{ "tracks/06.mp3" },
        AudioFile{ "tracks/07.mp3" },
        AudioFile{ "tracks/08.mp3" },
        AudioFile{ "tracks/09.mp3" },
        AudioFile{ "tracks/10.mp3" }
    };

    // Заполнение полей для тестов
    files[0].title = "TITLE";          files[0].artist = "Artist One";   files[0].album = "First Album";
    files[1].title = "Another Title";  files[1].artist = "Artist Two";   files[1].album = "Second Album";
    files[2].title = "Группа Крови";   files[2].artist = "Кино";         files[2].album = "Группа Крови";
    files[3].title = "TITLE SECOND";   files[3].artist = "Some Artist";  files[3].album = "Compilation";
    files[4].title = "Numb";           files[4].artist = "Linkin Park";  files[4].album = "Meteora";
    files[5].title = "title small";    files[5].artist = "Unknown";      files[5].album = "Single";
    files[6].title = "Звезда по имени Солнце"; files[6].artist = "Кино"; files[6].album = "Звезда...";
    files[7].title = "Shape of You";   files[7].artist = "Ed Sheeran";   files[7].album = "Divide";
    files[8].title = "Show Must Go On";files[8].artist = "Queen";        files[8].album = "Innuendo";
    files[9].title = "Девочка с каре"; files[9].artist = "МУККА";        files[9].album = "Single";

    FileFilter filter;

    SECTION("Return all non filtered files")
    {
        auto not_filtered_files = filter.Apply(files, "");
        REQUIRE(not_filtered_files.size() == files.size());
    }

    SECTION("Return filtered files with title 'TITLE'")
    {
        auto filtered_files = filter.Apply(files, "TITLE");
        REQUIRE(filtered_files.size() == 4);
    }
}
