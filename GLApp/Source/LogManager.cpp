#include "LogManager.h"


namespace RE
{

	std::shared_ptr<spdlog::logger> LogManager::s_Logger;
	
	void LogManager::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_Logger = spdlog::stdout_color_mt("RocketEngine");
		s_Logger->set_level(spdlog::level::trace);
		

		std::shared_ptr<MySink<std::mutex>> mysink(new MySink<std::mutex>);
		s_Logger->sinks().push_back(mysink);
	}

	void LogManager::Shutdown()
	{
		spdlog::shutdown();
	}
	
}


//Example 
// RE_INFO("Example Layer :: Update");
// RE_TRACE("{0}", event.ToString());