#include "DeleteItemActionStack.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/ECS/ECS_Manager.h"
#include "Engine/ECS/GameObject.h"

#include <algorithm>
#include <vector>

DeleteItemActionStack::DeleteItemActionStack(const std::string& desc, engine::Entity object)
	:ActionBehaviour{ desc }
{
	engine::GameObject& head = static_cast<engine::GameObject>(object);
	m_orignalParent = head.GetComponent<engine::Transform3D>().GetParentId();

	std::vector<engine::Entity> orignalCopy{ head };
	auto& orignalChild = head.GetChildren();
	orignalCopy.insert(orignalCopy.begin(), orignalChild.begin(), orignalChild.end());

	//engine::SceneManager::GetActiveWorld().StoreAsDeleted(head);
	//m_redoData.push_back(engine::SceneManager::GetActiveWorld().StoreAsDeleted(head));
	m_sparseHierarchy.emplace_back(0);
	for (engine::Entity child : orignalChild)
	{
		auto& iter = std::find(orignalCopy.begin(), orignalCopy.end(), child);
		m_sparseHierarchy.emplace_back(std::distance(orignalCopy.begin(), iter));
		//m_redoData.push_back(engine::SceneManager::GetActiveWorld().StoreAsDeleted(child));
	}

};
DeleteItemActionStack::~DeleteItemActionStack()
{
	m_redoData.clear();
	m_sparseHierarchy.clear();
};

void DeleteItemActionStack::undo()
{
	engine::SceneManager::GetActiveWorld().DestroyEntity(m_undoData);
}
void DeleteItemActionStack::redo()
{

	engine::World& activeWorld = engine::SceneManager::GetActiveWorld();
	m_undoData = activeWorld.RestoreFromDeleted(*m_redoData[0]);
	std::vector<engine::Entity> listCopy{ m_undoData };

	for (size_t i = 1; i < m_redoData.size(); ++i)
	{
		engine::GameObject go = activeWorld.RestoreFromDeleted(*m_redoData[i]);
		listCopy.emplace_back(go);
		engine::GameObject parent = listCopy[m_sparseHierarchy[i]];
		parent.AddChild(go);
	}
	static_cast<engine::GameObject>(m_orignalParent).AddChild(m_undoData);
};
