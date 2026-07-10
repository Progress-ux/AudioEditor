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

FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(ftxui Catch2)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TAGLIB REQUIRED taglib)

add_library(editor_core
    src/services/MetadataService.cpp
    src/services/FileScanner.cpp
)
target_include_directories(editor_core PUBLIC src)
target_link_libraries(editor_core PUBLIC ${TAGLIB_LIBRARIES})

add_executable(editor
    src/main.cpp

    src/app/App.cpp

    src/screens/EditorScreen.cpp
    src/screens/FileListScreen.cpp
)

target_link_libraries(editor PRIVATE
    editor_core
    ftxui::ftxui
)

enable_testing()

install(TARGETS editor RUNTIME DESTINATION bin)

add_executable(unit_tests
    tests/test_main.cpp
    tests/test_metadata.cpp
    tests/test_scanner.cpp
)

target_link_libraries(unit_tests PRIVATE
    editor_core 
    Catch2::Catch2WithMain
)

add_test(NAME AllTests COMMAND unit_tests)
