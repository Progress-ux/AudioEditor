#pragma once

#include "models/AudioFile.hpp"

#include <vector>

enum class AppScreen
{
    FileList = 0,
    Editor = 1
};

struct AppState
{
    std::vector<AudioFile> files;
    AudioFile current_file;

    int selectedFile = 0;

    AppScreen screen = AppScreen::FileList;
    int current_tab = 0;
    void changeScreen(AppScreen screen) 
    { 
        this->screen = screen;
        current_tab = static_cast<int>(screen);
    }
};
