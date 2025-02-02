/***********************************************************************************//*!
\file           GameSceneLayer.cpp
\project        INSERT PROJECT NAME
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Sept 06, 2021
\brief          The Main Layer that will run the actual game scene while editing.
                This means that certain systems dictated not to run during editing
                such as physics and scripting will not be updated here.
                Does not provide any functionality otherwise.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#include "GameSceneLayer.h"


//temporary code
//const char* filepath = "E:/Digipen/SchoolWork/GAM200/GameEngineBase/bin/Debug-OpenGL-windows-x86_64/Sandbox/scene/test.scn";

GameSceneLayer::GameSceneLayer(std::string const& filepath)
    : SceneBaseLayer{ "Game Scene Layer",""}
    , m_filepath{ filepath }
{
}

void GameSceneLayer::Init()
{
    //THIS SHOULD BE AN ATTEMPT LOAD AT STARTUP, IF FAILS CREATE NEW
    //temporary code
    ENGINE_ASSERT(std::filesystem::exists(m_filepath)
        , "this is testing code, the filepath eventually should always be valid.");
    /*if (!std::filesystem::exists(m_filepath))
        std::filesystem::create_directory(m_filepath);*/
    //m_scene.LoadFromFile(static_cast<std::string>(m_filepath));

    //Register All Systems
    auto& prefabSystem = GetWorld()->RegisterSystem<engine::PrefabComponentSystem>();
    auto& EditorSystem = GetWorld()->RegisterSystem<engine::EditorComponentSystem>();

    auto& RenderingSystem = GetWorld()->RegisterSystem<engine::Renderer2DSystem>(DefaultCamera());
    auto& ScriptingSystem = GetWorld()->RegisterSystem<engine::ScriptSystem>();
    auto& PhysicsSystem = GetWorld()->RegisterSystem<engine::PhysicsSystem>();

    // Randomly create ogre texture in the middle
    auto ogreHandle = engine::AssetManager::ImportAsset("../Engine/assets/images/ogre.png");
    auto tex = engine::AssetManager::GetAsset<engine::Texture>(ogreHandle);
    auto& testObject = CreateGameObject();
    auto& sprite = testObject.AddComponent<engine::Sprite2D>();
    sprite.SetTexture(tex);
    testObject.Transform().Scale() = { 50, 50, 1 };

}

void GameSceneLayer::Exit()
{
    SaveSceneToFile(m_filepath);
}

void GameSceneLayer::OnUpdate(engine::Timestep dt)
{
    ////only needed because of all the testing layers
    //engine::SceneManager::SetActiveScene(GetScene().GetID());
    //engine::WorldManager::SetActiveWorld(GetWorld()->GetID());

    //Only update things that will update while editing
    GetWorld()->GetSystem<engine::TransformSystem>()->Update();
    GetWorld()->GetSystem<engine::Renderer2DSystem>()->Update();
}

void GameSceneLayer::OnImGuiRender()
{
}
