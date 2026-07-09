#pragma once

#include "app/AppState.hpp"

#include <ftxui/component/app.hpp>
#include <string>
#include <vector>

class FileListScreen
{
private:
    std::vector<std::string> menu_entries_;
    AppState& state_;
    ftxui::Component menu_;
public:
    FileListScreen(AppState& state); 
    ftxui::Component GetComponent();
    void Refresh();
};
