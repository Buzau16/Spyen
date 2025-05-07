#pragma once
#include "Core/Log.h"

#define FMT_HEADER_ONLY
#include "fmt/format.h"

#ifndef SP_DISABLE_LOGGING
#define SPY_CORE_INFO(...)     ::Spyen::Log::CoreInfo(fmt::format(__VA_ARGS__))
#define SPY_CORE_WARN(...)     ::Spyen::Log::CoreWarn (fmt::format(__VA_ARGS__))
#define SPY_CORE_ERROR(...)    ::Spyen::Log::CoreError(fmt::format(__VA_ARGS__))
#define SPY_CORE_CRITICAL(...) ::Spyen::Log::CoreCritical(fmt::format(__VA_ARGS__))

#define SPY_INFO(...)		   ::Spyen::Log::ClientInfo(fmt::format(__VA_ARGS__))
#define SPY_WARN(...)		   ::Spyen::Log::ClientWarn (fmt::format(__VA_ARGS__))
#define SPY_ERROR(...)		   ::Spyen::Log::ClientError(fmt::format(__VA_ARGS__))
#define SPY_CRITICAL(...)	   ::Spyen::Log::ClientCritical(fmt::format(__VA_ARGS__))

#else
#define SPY_CORE_INFO(...)
#define SPY_CORE_WARN(...)
#define SPY_CORE_ERROR(...)
#define SPY_CORE_CRITICAL(...)

#define SPY_INFO(...)
#define SPY_WARN(...)
#define SPY_ERROR(...)
#define SPY_CRITICAL(...)

#endif

#define SPY_ASSERT(x, ...) { if(!(x)) { SPY_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define SPY_CORE_ASSERT(x, ...) { if(!(x)) { SPY_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }