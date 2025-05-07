#pragma once
#include <memory>
#include <string>
#include "Core/Core.h"

// Forward declare spdlog::logger to avoid including spdlog headers
namespace spdlog {
    class logger;
}


namespace Spyen {
    class SPYEN_API Log {
    public:
        static void Init();
        
        static void CoreInfo(const std::string& msg);
        static void CoreWarn(const std::string& msg);
        static void CoreError(const std::string& msg);
        static void CoreCritical(const std::string& msg);

        static void ClientInfo(const std::string& msg);
        static void ClientWarn(const std::string& msg);
        static void ClientError(const std::string& msg);
        static void ClientCritical(const std::string& msg);
    };
}