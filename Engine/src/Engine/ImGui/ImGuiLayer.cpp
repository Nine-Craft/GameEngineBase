/************************************************************************************//*!
\file           ImGuiLayer.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           May 25, 2021
\brief          Implements a basic ImGuiLayer that sets up for the core ImGui Features
				Required into the main application.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "pch.h"

#include "Engine/ImGui/ImGuiLayer.h"

#include "Engine/Core/Application.h"
#include <sdl2/SDL.h>

//#include <imgui.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <GL/gl3w.h>

namespace engine
{
	ImGuiLayer::ImGuiLayer()
		: m_blockEvents { true }
		, Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ENGINE_PROFILE_FUNCTION();

#ifdef ENGINE_PLATFORM_WINDOWS
		window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
		m_renderer = static_cast<SDL_Renderer*>(Application::Get().GetWindow().GetNativeRenderer());
#endif

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		ImGui::StyleColorsDark();
		
		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		
		//ImGui_ImplSDL2_Init(window, m_renderer);
		ImGui_ImplSDL2_InitForOpenGL(window, m_renderer);
		const char* glsl_version = "#version 130";
		ImGui_ImplOpenGL3_Init(glsl_version);
		//ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void ImGuiLayer::OnDetach()
	{
		ENGINE_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		//SDL_GL_DeleteContext(gl_context);
	}

	void ImGuiLayer::OnEvent(Event& e)
	{

		if (m_blockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();

			e.Handled |= e.IsInCategory(EVENT_CATEGORY::MOUSE) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EVENT_CATEGORY::KEYBOARD) & io.WantCaptureKeyboard;
			
		}
	}

	void ImGuiLayer::Begin()
	{
		ENGINE_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

	}

	void ImGuiLayer::End()
	{
		ENGINE_PROFILE_FUNCTION();

		//ImGuiIO& io = ImGui::GetIO();
		//Application& app = Application::Get();
		//io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		//// Rendering
		ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.5, 1, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}
		//SDL_GL_SwapWindow(window);
	}

}