#include "pch.h"
#include <rttr/registration>
#include "EditorComponent.h"
#include "EditorComponentSystem.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Asset/AssetsManager.h"
namespace engine
{

RTTR_REGISTRATION
{
	using namespace rttr;
registration::class_<EditorComponent>("Editor Component")
.property_readonly("Is Prefab", &EditorComponent::IsPrefab)
.property_readonly("Is Dirty", &EditorComponent::IsPrefabDirty)
.property_readonly("Is Shown", &EditorComponent::GetIsShownInEditor)
.property_readonly("Prefab Reference", &EditorComponent::GetPrefabReference)
.property("Object Icons", &EditorComponent::GetTexture, &EditorComponent::SetTexture);
}



void EditorComponent::BreakOffFromPrefab()
{
	engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->UnregisterUser(m_prefabReference,*this);
}

void EditorComponent::UpdatePrefab()
{
	engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->UpdatedPrefab(*this);
}
engine::EditorComponent::EditorComponent(engine::Entity entity, bool active)
	:Component{ entity,active },m_icon{ AssetManager::GetNamedAsset<engine::Texture>("Ouroboros_Prefab") }
{
}
Component& engine::EditorComponent::CopyComponent(Component const& comp)
{
	const EditorComponent& ec = reinterpret_cast<EditorComponent const &>(comp);
	m_isPrefab = ec.m_isPrefab;
	m_isPrefab_Dirty = ec.m_isPrefab_Dirty;

	if (m_prefabReference)
		engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->UnregisterUser(m_prefabReference, *this);
	m_prefabReference = ec.m_prefabReference;
	engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->RegisterNewUser(m_prefabReference, *this);

	m_headReference = 0;
	return *this;
};
bool EditorComponent::IsPrefab() const 
{
	return m_isPrefab; 
}
bool EditorComponent::IsPrefabDirty() const
{
	return m_isPrefab_Dirty;
}
engine::Entity EditorComponent::GetPrefabReference() const 
{
	return m_prefabReference; 
}
engine::Entity engine::EditorComponent::GetHeadReference() const 
{
	return m_headReference;
}
bool engine::EditorComponent::GetIsShownInEditor() const
{
    return m_isShown_inEditor;
}
std::shared_ptr<Texture> engine::EditorComponent::GetTexture() const
{
    return m_icon;
}
void EditorComponent::SetPrefabDirty(bool pd)
{

	m_isPrefab_Dirty = pd;
}
void engine::EditorComponent::SetIsPrefab(bool ip)
{
	m_isPrefab = ip;
}
void engine::EditorComponent::SetHead(Entity head)
{
	m_headReference = head;
}

void engine::EditorComponent::SetShownInEditor(bool isShown)
{
	m_isShown_inEditor = isShown;
}

void engine::EditorComponent::SetTexture(std::shared_ptr<Texture> texture)
{
	m_icon = texture;
}

void engine::EditorComponent::SetPrefabReference(Entity reference, Entity head)
{
	if (m_prefabReference)
		engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->UnregisterUser(m_prefabReference, *this);
	m_prefabReference = reference;
	engine::SceneManager::GetActiveWorld().GetSystem<EditorComponentSystem>()->RegisterNewUser(m_prefabReference, *this);
	m_isPrefab = true;
	m_headReference = head;
}

}