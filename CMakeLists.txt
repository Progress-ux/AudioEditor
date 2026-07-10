cmake_minimum_required(VERSION 3.15)

project(AudioEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v7.0.0
)
FetchContent_MakeAvailable(ftxui)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TAGLIB REQUIRED taglib)

add_executable(editor
    src/main.cpp

    src/app/App.cpp

    src/screens/EditorScreen.cpp
    src/screens/FileListScreen.cpp

    src/services/MetadataService.cpp
    src/services/FileScanner.cpp
)

target_include_directories(editor PRIVATE
    src
    ${TAGLIB_INCLUDE_DIRS}
)

target_link_libraries(editor PRIVATE
    ftxui::ftxui
    ${TAGLIB_LIBRARIES}
)

install(TARGETS editor RUNTIME DESTINATION bin)
