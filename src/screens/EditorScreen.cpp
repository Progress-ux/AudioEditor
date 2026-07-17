#include "EditorScreen.hpp"

#include "core/logger.hpp"
#include <exception>

#include "ftxui/dom/elements.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/deprecated.hpp>
#include <ftxui/dom/elements.hpp>

EditorScreen::EditorScreen(AppState& state, MetadataService& metadata)
    : state_(state), metadata_(metadata)
{
    title_input_ = ftxui::Input(&state_.current_file.title, "Title");
    artist_input_ = ftxui::Input(&state_.current_file.artist, "Artist");
    comment_input_ = ftxui::Input(&state_.current_file.comment, "Comment");
    album_input_ = ftxui::Input(&state_.current_file.album, "Album");
    genre_input_ = ftxui::Input(&state_.current_file.genre, "Genre");
    track_number_input_ = ftxui::Input(&state_.current_file.track_number, "Track number");
    date_input_ = ftxui::Input(&state_.current_file.date, "Date");

    save_button_ = ftxui::Button("Save", [&] {
        try 
        {
            metadata_.Save(state_.current_file);
            state_.files[state_.selectedFile] = state_.current_file;
            state_.changeScreen(AppScreen::FileList);
        }
        catch(std::exception& e)
        {
            LOG_ERROR("Save error: {}", e.what());
        }
    });
    cancel_button_ = ftxui::Button("Cancel", [&] {
        state_.changeScreen(AppScreen::FileList);
    });

    container_ = ftxui::Container::Vertical({
        title_input_,
        artist_input_,
        comment_input_,
        album_input_,
        genre_input_,
        date_input_,
        track_number_input_,
        ftxui::Container::Horizontal({ save_button_, cancel_button_ })
    });

    component_ = ftxui::Renderer(container_, [&] {
        return ftxui::vbox({
            ftxui::text("Edit file: " + state_.current_file.path.filename().string()) | ftxui::bold,
            ftxui::separator(),
            ftxui::hbox(ftxui::text("Title:          "), title_input_->Render()) | ftxui::border,
            ftxui::hbox(ftxui::text("Artist:         "), artist_input_->Render()) | ftxui::border,
            ftxui::hbox(ftxui::text("Comment:        "), comment_input_->Render()) | ftxui::border,
            ftxui::hbox(ftxui::text("Album:          "), album_input_->Render()) | ftxui::border,
            ftxui::hbox(ftxui::text("Genre:          "), genre_input_->Render()) | ftxui::border,
            ftxui::hbox(ftxui::text("Date:           "), date_input_->Render()) | ftxui::border,
            ftxui::hbox(ftxui::text("Track number:   "), track_number_input_->Render()) | ftxui::border,

            ftxui::vbox({
                ftxui::hbox({
                    save_button_->Render(),
                    ftxui::text("    "),
                    cancel_button_->Render()
                })
            }) | ftxui::center
        }) | ftxui::border;
    });
}

ftxui::Component EditorScreen::GetComponent()
{
    return component_;
}
