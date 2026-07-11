#include "services/FileScanner.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class SandboxFixture
{
private:
    fs::path path_;

    void clean()
    {
        if (fs::exists(path_))
        {
            fs::remove_all(path_);
        }
    }

public:
    SandboxFixture(const fs::path& path = "test_sandbox") : path_(path)
    {
        clean();
        fs::create_directory(path);
    }

    ~SandboxFixture()
    {
        clean();
    }

    const fs::path& path() const { return path_; }
};

void create_dummy_file(const fs::path& path)
{
    std::ofstream ofs(path);
}

 TEST_CASE("FileScanner: Сканирование директории", "[scanner]")
{
    FileScanner scanner;

    SECTION("Сканирование несуществующей папки вызывает исключение")
    {
        fs::path not_existing = "definitely_not_exists_sandbox";
        REQUIRE_THROWS_AS(scanner.Scan(not_existing), fs::filesystem_error);
    }

    SECTION("Сканирование пустой папки должно возвращать пустой вектор")
    {
        SandboxFixture sandbox;
        auto result = scanner.Scan(sandbox.path());

        REQUIRE(result.empty());
    }

    SECTION("Сканирование должно находить только файлы .mp3")
    {
        SandboxFixture sandbox;

        // Правильные файлы
        create_dummy_file(sandbox.path() / "song1.mp3");
        create_dummy_file(sandbox.path() / "song2.mp3");

        // Сканер должен проигнорировать
        create_dummy_file(sandbox.path() / "image.jpg");
        create_dummy_file(sandbox.path() / "readme.txt");
        create_dummy_file(sandbox.path() / "mp3_in_name_but_txt.mp3.txt");

        auto result = scanner.Scan(sandbox.path());

        REQUIRE(result.size() == 2);
    }
}
