#pragma once

#include "services/FileScanner.hpp"
#include "services/MetadataService.hpp"
#include "AppState.hpp"

#include <ftxui/component/screen_interactive.hpp>

class App 
{
private:
    ftxui::ScreenInteractive screen_;
    AppState state_;
    MetadataService metadata_;
    FileScanner scanner_;

public:
    App()
        : screen_(ftxui::ScreenInteractive::Fullscreen())
    {}

    void Run();
};
