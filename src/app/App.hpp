#pragma once

#include "screens/EditorScreen.hpp"
#include "services/FileScanner.hpp"
#include "services/MetadataService.hpp"
#include "AppState.hpp"

#include <filesystem>
#include <ftxui/component/screen_interactive.hpp>

class App 
{
private:
    ftxui::ScreenInteractive screen_;
    AppState state_;
    MetadataService metadata_;
    FileScanner scanner_;
    FileFilter filter_;
    std::filesystem::path folder_;

    std::filesystem::path ExpandUser(std::string);

public:
    App(std::string&);

    void Run();
};
