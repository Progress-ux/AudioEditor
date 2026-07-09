#include "FileListScreen.hpp"
#include "app/AppState.hpp"
#include "ftxui/dom/elements.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

FileListScreen::FileListScreen(AppState& state)
    : state_(state)
{
    Refresh();
    if (!menu_entries_.empty())
    {
        menu_ = ftxui::Menu(&menu_entries_, &state_.selectedFile);
    }
}

ftxui::Component FileListScreen::GetComponent()
{
    if (menu_entries_.empty())
    {
        return ftxui::Renderer([] {
            return ftxui::vbox({
                ftxui::text("No MP3 files found"),
                ftxui::separator(),
                ftxui::text("Press q to exit")
            });
        });
    }

    auto menu_handler = ftxui::CatchEvent(menu_, [&](ftxui::Event event) {
        if (event == ftxui::Event::Character('o'))
        {
            if (!state_.files.empty())
            {
                state_.current_file = state_.files[state_.selectedFile];
                state_.changeScreen(AppScreen::Editor);
            }
            return true;
        }
        return false;
    });

    return ftxui::Renderer(menu_handler, [menu_handler]() {
        return ftxui::vbox({
            ftxui::text("Enter file (j/k - up/down, o - edit, q - exit)"),
            ftxui::separator(),
            menu_handler->Render() | ftxui::frame | ftxui::border
        });
    });
}

void FileListScreen::Refresh()
{
    menu_entries_.clear();
    for (const auto& file : state_.files)
    {
        menu_entries_.push_back(file.path.filename().string());
    }
}
