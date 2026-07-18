cmake_minimum_required(VERSION 3.15)

project(AudioEditor LANGUAGES CXX)

if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "--------------------------------------------------------")
message(STATUS "Configuring AudioEditor with C++${CMAKE_CXX_STANDARD}...")
message(STATUS "--------------------------------------------------------")

include(FetchContent)

message(STATUS "[FetchContent] Declaring third-party dependencies...")
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v7.0.0
)

FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)

FetchContent_Declare(CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git 
    GIT_TAG v2.6.0
)

FetchContent_Declare(spdlog 
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)

message(STATUS "[FetchContent] Fetching and making dependencies available...")
FetchContent_MakeAvailable(ftxui Catch2 CLI11 spdlog)

find_package(Boost REQUIRED CONFIG COMPONENTS locale)

if(WIN32)
    find_package(ZLIB REQUIRED)
    find_package(taglib CONFIG REQUIRED)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(TAGLIB REQUIRED taglib)
endif()

message(STATUS "[PkgConfig] TagLib library found!")
message(STATUS "  - Version:  ${TAGLIB_VERSION}")
message(STATUS "  - Includes: ${TAGLIB_INCLUDE_DIRS}")
message(STATUS "  - Libs:     ${TAGLIB_LIBRARIES}")

add_library(editor_core
    src/services/MetadataService.cpp
    src/services/FileScanner.cpp

    src/filters/FileFilter.cpp
    src/utils/Utils.cpp
)
target_include_directories(editor_core PUBLIC src)

target_link_libraries(editor_core PUBLIC 
    spdlog::spdlog_header_only
    Boost::locale
)

if(WIN32)
    target_link_libraries(editor_core PUBLIC TagLib::tag)
else()
    target_link_libraries(editor_core PUBLIC ${TAGLIB_LIBRARIES})
endif()

add_executable(editor
    src/main.cpp

    src/app/App.cpp

    src/screens/EditorScreen.cpp
    src/screens/FileListScreen.cpp
)

target_link_libraries(editor PRIVATE
    editor_core
    ftxui::ftxui
    CLI11::CLI11
)

enable_testing()

if(CMAKE_TESTING_ENABLED)
    message(STATUS "[Testing] Configuring Unit Tests...")

    add_executable(unit_tests
        tests/test_main.cpp
        tests/test_metadata.cpp
        tests/test_scanner.cpp
        tests/test_filter.cpp
    )

    target_link_libraries(unit_tests PRIVATE
        editor_core 
        Catch2::Catch2WithMain
    )

    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    include(Catch)
    catch_discover_tests(unit_tests)
endif()

install(TARGETS editor RUNTIME DESTINATION bin)
message(STATUS "Configuration finished successfully.")
