#include "FileListScreen.hpp"
#include "app/AppState.hpp"
#include "ftxui/dom/elements.hpp"
#include "screens/EditorScreen.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

FileListScreen::FileListScreen(AppState& state, FileFilter& filter, std::function<void()> exit)
    : state_(state), filter_(filter), exit_(std::move(exit))
{
    search_input_ = ftxui::Input(&state_.search_query, " Search...");

    Refresh();

    menu_ = ftxui::Menu(&menu_entries_, &state_.selectedFile);
}

ftxui::Component FileListScreen::GetComponent()
{
    auto dynamic_container = ftxui::Container::Vertical({
        search_input_,
        menu_
    });

    menu_->TakeFocus();

    auto menu_handler = ftxui::CatchEvent(dynamic_container, [this, dynamic_container](ftxui::Event event) {
        if (event == ftxui::Event::Character('/'))
        {
            search_input_->TakeFocus();
            return true;
        }

        if (event == ftxui::Event::Escape)
        {
            menu_->TakeFocus();
            return true;
        }

        std::string old_query = state_.search_query;
        bool handled = dynamic_container->OnEvent(event);

        if (handled)
        {
            if (state_.search_query != old_query)
            {
                state_.visible_files =
                    filter_.Apply(state_.all_files, state_.search_query);

                Refresh();

                if (!menu_entries_.empty() && 
                        state_.selectedFile >= menu_entries_.size())
                {
                    state_.selectedFile = static_cast<int>(menu_entries_.size()) - 1;
                }
            }
        }

        if (!search_input_->Focused() &&
            event == ftxui::Event::Character('o'))
        {
            if (!state_.visible_files.empty())
            {
                state_.current_file = state_.visible_files[state_.selectedFile];
                state_.changeScreen(AppScreen::Editor);
            }
            return true;
        }

        if (!search_input_->Focused() &&
            event == ftxui::Event::Character('q'))
        {
            exit_();
            return true;
        }

        return handled;
    });

    return ftxui::Renderer(menu_handler, [this] {
        ftxui::Element content;

        content = menu_->Render() | ftxui::frame;
        if (state_.visible_files.empty())
        {
            content = ftxui::vbox({
                ftxui::vbox({
                    ftxui::text("Not found") | ftxui::color(ftxui::Color::Red),
                }) | ftxui::center | ftxui::flex
            });
        }

        return ftxui::vbox({
            ftxui::text("Navigation: k/j - up/down, / - search, o - edit file, q - exit"),
            ftxui::separator(),
            search_input_->Render(),
            ftxui::separator(),
            content | ftxui::border | ftxui::flex
        });
    });
}

void FileListScreen::Refresh()
{
    menu_entries_.clear();
    for (const auto& file : state_.visible_files)
    {
        menu_entries_.push_back(file.path.filename().string());
    }
}
