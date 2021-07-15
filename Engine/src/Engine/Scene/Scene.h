/*****************************************************************//**
\file       Scene.h
\project    INSERT_PROJECT_HERE
\author     Lim Guan Hui, l.guanhui , 2000552
\par        email: l.guanhui\@digipen.edu
\date       15/7/2021
\brief  
Scene is a class that can load a World from a scene file, thus
creating a World with information contained in the scene file, 
and also save a loaded world's information into a scene file.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
 *********************************************************************/
#pragma once
#include "Engine/ECS/WorldManager.h"
#include <string>
namespace engine
{
	class Scene
	{
		friend class SceneManager;
	private:

		explicit Scene(std::string filename) : m_filename{ std::move(filename) } {};
		~Scene() = default;


	private:
		std::string m_filename{};
		World* m_world = nullptr;
	public:


		std::string GetSceneName()
		{
			return m_filename.substr(0, m_filename.find_first_of('.'));
		}

		World& Load()
		{
			m_world = &WorldManager::CreateWorld();

			//deserialise scene file and load objects here


			return *m_world;
		}

		bool IsLoaded()
		{
			return m_world != nullptr;
		}

		void Unload()
		{
			//save data to file here

			if (m_world)
				WorldManager::DestroyWorld(m_world->GetID());
			m_world = nullptr;
		}

		World& GetWorld()
		{
			ENGINE_ASSERT(m_world != nullptr);
			return *m_world;
		}
	};
}