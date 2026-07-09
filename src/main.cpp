#include <filesystem>
#include <ftxui/component/event.hpp>
#include <string>
#include <vector>
#include <taglib/fileref.h>
#include <taglib/tag.h>

#include "ftxui/component/app.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

enum class AppScreen
{
    FileList,
    Editor
};

void load_file_metadata(const std::string& filepath, std::string& title, std::string& artist) 
{
    TagLib::FileRef f(filepath.c_str());
    if (!f.isNull() && f.tag()) 
    {
        TagLib::Tag* tag = f.tag();
        title = tag->title().to8Bit(true);
        artist = tag->artist().to8Bit(true);
    }

    if (title.empty())
    {
        title = std::filesystem::path(filepath).filename().string();
    }
    if (artist.empty())
    {
        artist = "Unknow artist";
    }
}

void save_file_metadata(const std::string& filepath, const std::string& title, const std::string& artist)
{
    TagLib::FileRef f(filepath.c_str());
    if (!f.isNull() && f.tag())
    {
        f.tag()->setTitle(TagLib::String(title, TagLib::String::UTF8));
        f.tag()->setArtist(TagLib::String(artist, TagLib::String::UTF8));
        f.save();
    }
}

int main(int argc, char *argv[])
{
    std::vector<std::string> files;
    int selected_file_idx = 0;

    std::string input_title = "";
    std::string input_artist = "";

    AppScreen current_screen = AppScreen::FileList;
    int tab_index = 0;

    for (const auto &entry : std::filesystem::directory_iterator("."))
    {
        std::string filename = entry.path().filename();
        if (filename.ends_with(".mp3"))
        {
            files.push_back(entry.path().filename());
        }
    }

    auto screen = ftxui::ScreenInteractive::Fullscreen();

    // First screen: List files

    auto file_menu = ftxui::Menu(&files, &selected_file_idx);

    auto file_menu_handler = ftxui::CatchEvent(file_menu, [&](ftxui::Event event) {
        if (event == ftxui::Event::Character('q'))
        {
            screen.ExitLoopClosure()();
            return true;
        }
        if (event == ftxui::Event::Character('o'))
        {
            if (!files.empty())
            {
                load_file_metadata(files[selected_file_idx], input_title, input_artist);

                tab_index = 1;
            }
            return true;
        }
        return false;
    });

    auto file_list_renderer = ftxui::Renderer(file_menu_handler, [&] {
        return ftxui::vbox({
            ftxui::text("Enter file (j/k - up/down, o - edit, q - exit)"),
            ftxui::separator(),
            file_menu_handler->Render() | ftxui::frame | ftxui::border
        });
    });

    // Second screen: Edit data

    auto input_title_comp = ftxui::Input(&input_title, "Title");
    auto input_artist_comp = ftxui::Input(&input_artist, "Artist");

    auto save_btn = ftxui::Button("Save", [&] { 
        save_file_metadata(files[selected_file_idx], input_title, input_artist);
        tab_index = 0; 
    });
    auto cancel_btn = ftxui::Button("Cancel", [&] { 
        tab_index = 0; 
    });

    auto editor_container = ftxui::Container::Vertical({
        input_title_comp, 
        input_artist_comp,
        ftxui::Container::Horizontal({save_btn, cancel_btn})
    });

    auto editor_renderer = ftxui::Renderer(editor_container, [&] {
        return ftxui::vbox({
            ftxui::text("Edit file: " + files[selected_file_idx]) | ftxui::bold,
            ftxui::separator(),
            ftxui::hbox(ftxui::text("Title:     "), input_title_comp->Render()) | ftxui::border,
            ftxui::hbox(ftxui::text("Artist:    "), input_artist_comp->Render()) | ftxui::border,
            ftxui::vbox({
                ftxui::hbox({
                    save_btn->Render(),
                    ftxui::text("  "),
                    cancel_btn->Render()
                })
            }) | ftxui::center 
        }) | ftxui::border;
    });

    auto tabs = ftxui::Container::Tab({
        file_list_renderer,
        editor_renderer
    }, &tab_index);

    screen.Loop(tabs);

    return 0;
}
