/*****************************************************************//**
 * \file   LoggingView.cpp
 * \brief  Logs the console outputs into a window
 *		   Logs the console output from scripts
 * 
 * \author Leong Jun Xiang (junxiang.leong)
 * \date   June 2021
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/

#include "LoggingView.h"

#include <imgui.h>
#include <sstream>
#include <iostream>

// required to obtain the ostringstream to read its message
#include <Engine/Core/Log.h>
#include <sstream>

std::deque<std::string> LoggingView::s_messages;

void LoggingView::Show()
{
	ImGui::Begin("Logger");
	ImGui::Text(::engine::Log::GetOstreamOutput().str().c_str());
	/*for (std::string& s : s_messages)
		ImGui::Text(s.c_str());*/
	ImGui::End();
	/* uncomment for a temp solution which clears the ostream every frame.
	::engine::Log::oss.str("")
	::engine::Log::oss.clear(); */
}

//this is now unused
void LoggingView::AddLoggingMsg(const char* fmt, ...)
{
	char buf[1024];
	va_list vargs;
	va_start(vargs, fmt);
	vsnprintf(buf, sizeof(buf), fmt, vargs);
	buf[sizeof(buf) - 1] = '\0';
	s_messages.emplace_back(buf);
}
