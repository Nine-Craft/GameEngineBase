#pragma once

#include "Engine/Core/Input.h"

#include <sdl2/SDL.h>

#include "Platform/Windows/WindowsWindow.h"

namespace engine
{
    class WindowsInput final : public Input
    {
    public:
        WindowsInput();
        virtual ~WindowsInput();

    protected:

        void Update() override;

        bool IsKeyDownImpl(KeyCode keycode) override;
        bool IsKeyPressedImpl(KeyCode keycode) override;
        bool IsKeyReleasedImpl(KeyCode keycode) override;
        
        bool IsMouseButtonDownImpl(MouseCode button) override;
        bool IsMouseButtonPressedImpl(MouseCode button) override;
        bool IsMouseButtonReleasedImpl(MouseCode button) override;

        std::pair<int, int> GetMousePositionImpl() override;
        int GetMouseXImpl() override;
        int GetMouseYImpl() override;

    private :
        int m_keyLength;
        const Uint8* m_keyboardState;
        Uint8* m_prevKeyboardState;

        Uint32 m_prevMouseState;
        Uint32 m_mouseState;

        int m_mouseXPos;
        int m_mouseYPos;
    };
}