#include <filesystem>
#include <string>
#include <vector>

#include "ftxui/component/app.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"

int main(int argc, char *argv[])
{
    std::vector<std::string> entries;
    int selected = 0;

    for (const auto &entry : std::filesystem::directory_iterator("."))
    {
        std::string filename = entry.path().filename();
        if (filename.ends_with(".mp3"))
        {
            entries.push_back(entry.path().filename());
        }
    }

    auto radiobox = ftxui::Menu(&entries, &selected);
    auto renderer = ftxui::Renderer(
        radiobox,
        [&]
        {
            return radiobox->Render() | ftxui::vscroll_indicator |
                   ftxui::frame |
                   ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 10) |
                   ftxui::border;
        });

    auto screen = ftxui::App::FitComponent();
    screen.Loop(renderer);

    return 0;
}
