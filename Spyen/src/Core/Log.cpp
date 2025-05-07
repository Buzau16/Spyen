//
// Created by gpro9 on 5/7/2025.
//

#include <filesystem>
#include <string>
#include <vector>
#include <Core/Log.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Spyen {
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;

    void DeleteExcessLogFiles() {
        uint32_t fileCount = 0;
        using Dir_Iterator = std::filesystem::directory_iterator;
        std::vector<std::filesystem::directory_entry> files;
        for (auto& file : Dir_Iterator("logs")) {
            //SPY_CORE_INFO("File in logs : {0}", file.path().string());
            files.push_back(file);
            ++fileCount;
        }

        if (fileCount > 10) {
            std::sort(files.begin(), files.end(),
                [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
                return std::filesystem::last_write_time(a) < std::filesystem::last_write_time(b);
                });
            for (size_t i = 0; i < files.size() - 10; i++) {
                //SPY_CORE_INFO("Deleting log file: {0}", files[i].path().string());
                std::filesystem::remove(files[i].path());
            }
        }
    }

    std::string GetTimestampedFilename()
    {
        const auto now = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        std::ostringstream oss;
        oss << "logs/Spyen-"
            << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S")
            << ".log";
        return oss.str();
    }

    void Log::Init()
    {
        std::vector<spdlog::sink_ptr> logSinks;
        // Colored console sink
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        // File sink
        std::string filename = GetTimestampedFilename();

        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true));

        // Set the pattern for both sinks (as shown in your Log.cpp)
        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        // Create loggers with multiple sinks
        s_CoreLogger = std::make_shared<spdlog::logger>("SPYEN", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_CoreLogger);
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace);

        s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);

        DeleteExcessLogFiles();
    }

    // Core logging
    void Log::CoreInfo(const std::string& msg) {
        s_CoreLogger->info(msg);
    }

    void Log::CoreWarn(const std::string& msg) {
        s_CoreLogger->warn(msg);
    }

    void Log::CoreError(const std::string& msg) {
        s_CoreLogger->error(msg);
    }

    void Log::CoreCritical(const std::string& msg) {
        s_CoreLogger->critical(msg);
    }

    // Client logging
    void Log::ClientInfo(const std::string& msg) {
        s_ClientLogger->info(msg);
    }

    void Log::ClientWarn(const std::string& msg) {
        s_ClientLogger->warn(msg);
    }

    void Log::ClientError(const std::string& msg) {
        s_ClientLogger->error(msg);
    }

    void Log::ClientCritical(const std::string& msg) {
        s_ClientLogger->critical(msg);
    }
} // Spyen
