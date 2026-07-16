#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "core/logger.hpp"
#include "CLI/CLI.hpp"
#include "app/App.hpp"

int main(int argc, char *argv[])
{
    Logger::init();
    LOG_INFO("Program started");

    CLI::App parser { "Audio Editor Command Line Interface" };

    std::string path;
    parser.add_option<std::string>("--path,-p", path, "Path to the directory");

    CLI11_PARSE(parser, argc, argv);

    App app(path);
    app.Run();

    Logger::shutdown();
    return 0;
}
