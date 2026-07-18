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
            state_.all_files.push_back(audio_file);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("Skip file with error: {}", e.what());
            continue;
        }
    }
    state_.visible_files = state_.all_files;

    FileListScreen list_screen(state_, filter_, screen_.ExitLoopClosure());
    auto list_component = list_screen.GetComponent();

    EditorScreen editor_screen(state_, metadata_, filter_);
    auto editor_component = editor_screen.GetComponent();

    auto tabs = ftxui::Container::Tab({
        list_component,
        editor_component
    }, &state_.current_tab);

    screen_.Loop(tabs);
}
