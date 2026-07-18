#pragma once

#include "services/MetadataService.hpp"
#include "filters/FileFilter.hpp"
#include "app/AppState.hpp"

#include <ftxui/component/app.hpp>

class EditorScreen 
{
private:
    AppState& state_;
    MetadataService& metadata_;
    ftxui::Component component_;

    ftxui::Component title_input_;
    ftxui::Component artist_input_;
    ftxui::Component comment_input_;
    ftxui::Component album_input_;
    ftxui::Component genre_input_;
    ftxui::Component date_input_;
    ftxui::Component track_number_input_;

    ftxui::Component save_button_;
    ftxui::Component cancel_button_;
    ftxui::Component container_;

public:
    EditorScreen(AppState& state, MetadataService& metadata, FileFilter& filter);
    ftxui::Component GetComponent();
};
