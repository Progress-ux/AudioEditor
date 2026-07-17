#include "App.hpp"
#include "core/logger.hpp"
#include "screens/EditorScreen.hpp"
#include "screens/FileListScreen.hpp"

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include <iostream>
#include <utility>

std::filesystem::path App::ExpandUser(std::string path)
{
    if (path.starts_with("~"))
    {
        if (const char* home = std::getenv("HOME"))
            path.replace(0, 1, home);
    }
    return path;
}

App::App(std::string& path)
    : screen_(ftxui::ScreenInteractive::Fullscreen())
{
    if(path.empty())
    {
        folder_ = ".";
        return;
    }

    std::filesystem::path targetPath(ExpandUser(path));
    if (std::filesystem::exists(targetPath) && std::filesystem::is_directory(targetPath))
    {
        folder_ = std::move(targetPath);
    }
    else 
    {
        folder_ = ".";
    }
}


void App::Run()
{
    std::vector<std::filesystem::path> files;
    try 
    {
        files = scanner_.Scan(folder_);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Scan error: " << e.what() << "\n";
        LOG_ERROR("Scan error: {}", e.what());
        return;
    }

    for (const auto& file : files)
    {
        try 
        {
            AudioFile audio_file = metadata_.Load(file);
            state_.files.push_back(audio_file);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("Skip file with error: {}", e.what());
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
