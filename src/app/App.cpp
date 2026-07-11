#include "App.hpp"
#include "screens/EditorScreen.hpp"
#include "screens/FileListScreen.hpp"

#include <exception>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <iostream>

void App::Run()
{
    std::vector<std::filesystem::path> files;
    try 
    {
        files = scanner_.Scan(".");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Scan error: " << e.what() << "\n";
        return;
    }

    for (const auto& file : files)
    {
        try 
        {
            AudioFile audio_file = metadata_.Load(file);
            state_.files.push_back(audio_file);
        }
        catch (...)
        {
            continue;
        }
    }

    FileListScreen list_screen(state_);
    auto list_component = list_screen.GetComponent();

    EditorScreen editor_screen(state_, metadata_);
    auto editor_component = editor_screen.GetComponent();

    auto tabs = ftxui::Container::Tab({
        list_component,
        editor_component
    }, &state_.current_tab);

    auto root = ftxui::CatchEvent(tabs, [&](ftxui::Event event) {
        if (event == ftxui::Event::Character('q'))
        {
            screen_.ExitLoopClosure()();
            return true;
        }
        if (event == ftxui::Event::Escape && state_.current_tab == 1)
        {
            state_.changeScreen(AppScreen::FileList);
            return true;
        }
        return false;
    });

    screen_.Loop(root);
}
