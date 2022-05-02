#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "MySink.h"

namespace RE {

	class LogManager
	{
	public:
		static void Init();
		void Shutdown();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

#define RE_ERROR(...)  RE::LogManager::GetLogger()->error(__VA_ARGS__);
#define RE_WARN(...)   RE::LogManager::GetLogger()->warn(__VA_ARGS__);
#define RE_INFO(...)   RE::LogManager::GetLogger()->info(__VA_ARGS__);
#define RE_TRACE(...)  RE::LogManager::GetLogger()->trace(__VA_ARGS__);
#define RE_FATAL(...)  RE::LogManager::GetLogger()->fatal(__VA_ARGS__);
