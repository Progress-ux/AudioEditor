#include "services/FileScanner.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void create_dummy_file(const fs::path& path)
{
    std::ofstream ofs(path);
}

 TEST_CASE("FileScanner: Сканирование директории", "[scanner]")
{
    FileScanner scanner;
    fs::path sandbox = "test_sandbox";

    if (fs::exists(sandbox))
    {
        fs::remove_all(sandbox);
    }
    fs::create_directory(sandbox);

    SECTION("Сканирование пустой папки должно возвращать пустой вектор")
    {
        auto result = scanner.Scan(sandbox);

        REQUIRE(result.empty());
    }

    SECTION("Сканирование должно находить только файлы .mp3")
    {
        // Правильные файлы
        create_dummy_file(sandbox / "song1.mp3");
        create_dummy_file(sandbox / "song2.mp3");

        // Сканер должен проигнорировать
        create_dummy_file(sandbox / "image.jpg");
        create_dummy_file(sandbox / "readme.txt");
        create_dummy_file(sandbox / "mp3_in_name_but_txt.mp3.txt");

        auto result = scanner.Scan(sandbox);


        REQUIRE(result.size() == 2);
    }

    fs::remove_all(sandbox);
    
}
