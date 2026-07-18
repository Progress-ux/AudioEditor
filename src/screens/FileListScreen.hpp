#pragma once

#include "app/AppState.hpp"
#include "screens/EditorScreen.hpp"

#include <ftxui/component/app.hpp>
#include <functional>
#include <string>
#include <vector>

class FileListScreen
{
private:
    std::vector<std::string> menu_entries_;
    AppState& state_;
    ftxui::Component menu_;
    ftxui::Component search_input_;
    ftxui::Component container_;
    FileFilter filter_;
    std::function<void()> exit_;
public:
    FileListScreen(AppState& state, FileFilter& filter, std::function<void()> exit); 
    ftxui::Component GetComponent();
    void Refresh();
};
