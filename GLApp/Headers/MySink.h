#pragma once
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/details/null_mutex.h"
#include "GLAppUI.h"
#include <iostream>
#include <mutex>

namespace RE
{
	template<typename Mutex>
	class MySink: public spdlog::sinks::base_sink <Mutex>
	{
	public:
        void sink_it_(const spdlog::details::log_msg& msg)
        {
            GLAppUI gla;
           
            // If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);                         
            str_vec.push_back(fmt::to_string(formatted));
            gla.SetLogVector(str_vec);
       
        }
        void flush_()
        {
            (void)0;
        }
    private:
         std::vector<std::string> str_vec;
   };
}