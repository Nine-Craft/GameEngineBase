/************************************************************************************//*!
\file           ApplicationEvent.h
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 15, 2021
\brief

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/Events/Event.h"

namespace engine
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_width{ width }, m_height{ height } {}

        unsigned int GetWidth() const { return m_width; }
        unsigned int GetHeight() const { return m_height; }

        std::string ToString() const override 
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_width << ", " << m_height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::Application })

    private:
        unsigned int m_width, m_height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;
        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::Application })
    };

    class WindowFocusEvent : public Event
    {
    public:
        WindowFocusEvent() = default;
        EVENT_CLASS_TYPE(WindowFocus)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::Application })
    };

    class WindowLoseFocusEvent : public Event
    {
    public:
        WindowLoseFocusEvent() = default;
        EVENT_CLASS_TYPE(WindowLoseFocus)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::Application })
    };

    class WindowMovedEvent : public Event
    {
    public:
        WindowMovedEvent() = default;
        EVENT_CLASS_TYPE(WindowMoved)
        EVENT_CLASS_CATEGORY(utility::bitmask{ EVENT_CATEGORY::Application })
    };

}