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

TEST_CASE("FileScanner: Directory Scanning", "[scanner]")
{
    FileScanner scanner;

    SECTION("Scanning a non-existent directory throws an exception")
    {
        fs::path not_existing = "definitely_not_exists_sandbox";
        REQUIRE_THROWS_AS(scanner.Scan(not_existing), fs::filesystem_error);
    }

    SECTION("Scanning an empty directory should return an empty vector")
    {
        SandboxFixture sandbox;
        auto result = scanner.Scan(sandbox.path());

        REQUIRE(result.empty());
    }

    SECTION("Scanning should find only .mp3 files")
    {
        SandboxFixture sandbox;

        // Valid files
        create_dummy_file(sandbox.path() / "song1.mp3");
        create_dummy_file(sandbox.path() / "song2.mp3");

        // Scanner should ignore these
        create_dummy_file(sandbox.path() / "image.jpg");
        create_dummy_file(sandbox.path() / "readme.txt");
        create_dummy_file(sandbox.path() / "mp3_in_name_but_txt.mp3.txt");

        auto result = scanner.Scan(sandbox.path());

        REQUIRE(result.size() == 2);
    }
}
