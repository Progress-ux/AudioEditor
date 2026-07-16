#pragma once 

#include <cstdio>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

#include <cstdlib>
#include <filesystem>

class Logger 
{
private: 
    static std::string getLogPath()
    {
        std::filesystem::path base_path;
#if defined (_WIN32)
        const char* appData = std::getenv("LOCALAPPDATA");
        if (appData)
            base_path = std::filesystem::path(appData) / "AudioEditor";
        else 
            base_path = ".";
#else 
        const char* home = std::getenv("HOME");
        if (home)
            base_path = std::filesystem::path(home) / ".local" / "state" / "AudioEditor";
        else 
            base_path = ".";
#endif
        std::filesystem::create_directories(base_path);
        return (base_path / "audio_editor.log").string();
    }

public:
    static void init()
    {
        try
        {
            spdlog::init_thread_pool(8192, 1);

            std::string log_file_path = getLogPath();
            auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_file_path, 0, 0);

            file_sink->set_level(spdlog::level::debug);
            file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%#] %v");

            std::vector<spdlog::sink_ptr> sinks { file_sink };

            auto logger = std::make_shared<spdlog::async_logger>(
                "global_async_logger", 
                sinks.begin(), 
                sinks.end(), 
                spdlog::thread_pool(), 
                spdlog::async_overflow_policy::block 
            );

            spdlog::register_logger(logger);
            spdlog::set_default_logger(logger);

            spdlog::set_level(spdlog::level::debug);

            spdlog::flush_on(spdlog::level::err);
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            std::printf("Logger initialization failed: %s\n", ex.what());
        }
    }
    static void shutdown()
    {
        spdlog::shutdown();
    }

};

#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__)
#define LOG_INFO(...)  SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__)
#define LOG_WARN(...)  SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__)
