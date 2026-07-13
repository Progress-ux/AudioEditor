#include "CLI/CLI.hpp"
#include "app/App.hpp"

int main(int argc, char *argv[])
{
    CLI::App parser { "Audio Editor Command Line Interface" };

    std::string path;
    parser.add_option<std::string>("--path,-p", path, "Path to the directory");

    CLI11_PARSE(parser, argc, argv);

    App app(path);
    app.Run();
    return 0;
}
