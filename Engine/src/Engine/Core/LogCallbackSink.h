/*****************************************************************//**
 * \file   LogCallbackSink.h
 * \brief  a spdlog sink which allows for callback when using the 
 *		   logging functions
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   12 July 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include <spdlog/sinks/base_sink.h>
#include <functional>
#include <string>
#include <vector>
namespace engine
{
	using FunctionType = std::function<void(const std::string&, char,const std::string&)>;
	template <typename Mutex>
	class CallbackSink : public spdlog::sinks::base_sink <Mutex>
	{
	public:
		/**
		 * \brief 
		 * construction sets the formatting for the sink
		 */
		CallbackSink() 
		{
			this->set_pattern("%n: %v%$");
		}
		/**
		 * \brief 
		 * subscribe to the current sink when the string gets formatted
		 */
		static void SubscribeToSink(FunctionType subscriber)
		{
			m_subscriberList.emplace_back(subscriber);
		}
	protected:
		/**
		 * \brief action performed when there is a 
		 * 
		 * \param msg
		 *  contains info about the log message
		 */
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
			std::string temp = fmt::to_string(formatted);
			
			for (auto& subscriber : m_subscriberList)
			{
				subscriber(temp,(char)msg.level,msg.source.filename);
			}
		}

		void flush_() override
		{
			
		}
	private:
		static std::vector < FunctionType > m_subscriberList;
	};
	template<typename mutex>
	std::vector < FunctionType > CallbackSink<mutex>::m_subscriberList;
}
#include "spdlog/details/null_mutex.h"
#include <mutex>
using CallbackSink_mt = engine::CallbackSink<std::mutex>;
using CallbackSink_st = engine::CallbackSink<spdlog::details::null_mutex>;