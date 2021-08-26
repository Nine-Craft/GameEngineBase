#include "pch.h"

#include "Engine/Scene/SceneManager.h"
#include "PrefabComponentSystem.h"
#include "PrefabComponent.h"
#include "../../Sandbox/src/Editor/Component/EditorComponent.h"
#include "Utility/Hash.h"
#include "Seralizer.h"
namespace engine
{
void PrefabComponentSystem::SavePrefab()
{
	//currently just forcefully save all prefabs
	for (auto& iter : m_prefabDetails)
	{
		Serializer::SaveObject(iter.second.filename);
	}
}

Entity PrefabComponentSystem::AddPrefab(const std::string& filepath)
{
    utility::StringHash::size_type fileHash = utility::StringHash::GenerateFNV1aHash(filepath);
	auto& iter = m_prefabDetails.find(fileHash);
	if (iter != m_prefabDetails.end())
		return iter->second.head;

	//use serializer to serialize prefab
	Entity headNode = Serializer::LoadObject(filepath, -1);//not shown in hierarchy
	ENGINE_ASSERT(headNode != 0);//means seralization failed

	static_cast<GameObject>(headNode).GetComponent<PrefabComponent>().m_RootNode = true;
	m_prefabDetails[fileHash] = FileDetails{ headNode,filepath };
	return headNode;
}

Entity PrefabComponentSystem::GetPrefab(const std::string& filepath)
{
	utility::StringHash::size_type fileHash = utility::StringHash::GenerateFNV1aHash(filepath);
	auto& item =  m_prefabDetails.find(fileHash);
	if (item != m_prefabDetails.end())
		return 0;

	return item->second.head;
}

void PrefabComponentSystem::InstantiateFromPrefab(const std::string& filepath, GameObject& head)
{
	Entity obj = AddPrefab(filepath);
	GameObject& GO = static_cast<GameObject>(obj);
	Transform3D& trans = GO.GetComponent<Transform3D>();
	
	Transform3D& headTrans = head.GetComponent<Transform3D>();
	headTrans.SetPosition(trans.GetPosition());
	headTrans.SetRotationAngle(trans.GetRotationAngle());
	headTrans.SetRotationAxis(trans.GetRotationAxis());
	headTrans.SetScale(trans.GetScale());

	auto& childList = GO.GetChildren();
	std::vector<Entity> orignal{GO};
	std::vector<Entity> current{head};
	Entity prevParent = head;

	auto headEditorComponent = head.TryGetComponent<EditorComponent>();
	if (headEditorComponent)
	{
		headEditorComponent->SetPrefab(true);
		headEditorComponent->SetPrefabReference(obj);
	}

	for (GameObject childs : childList)
	{
		engine::GameObject		child = engine::SceneManager::GetActiveScene().CreateGameObject();
		engine::GameObject& copyObject = childs;
		engine::Transform3D& copyTransform = copyObject.GetComponent<engine::Transform3D>();
		//gameobject component
		child.Name() = copyObject.Name();
		child.ActiveSelf() = static_cast<bool>(copyObject.ActiveSelf());
		auto editorComponent = child.TryGetComponent<EditorComponent>();
		if(editorComponent)
		{
			editorComponent->SetPrefab(true);
			editorComponent->SetPrefabReference(copyObject.GetID());
		}

		{//TODO fix this once its done
			engine::Transform3D& newTrans = static_cast<engine::GameObject>(copyObject).GetComponent<engine::Transform3D>();
			engine::Transform3D& childTrans = child.GetComponent<engine::Transform3D>();
			//childTrans.CopyComponent(newTrans);
			childTrans.SetPosition(newTrans.GetPosition());
			childTrans.SetScale(newTrans.GetScale());
			childTrans.SetRotationAngle(newTrans.GetRotationAngle());
			childTrans.SetRotationAxis(newTrans.GetRotationAxis());
		}

		const engine::Entity parentid = copyTransform.GetParentId();

		{
			auto& iter = std::find(orignal.begin(), orignal.end(), parentid);
			if (iter != orignal.end())//means there is a result
			{
				orignal.erase(++iter, orignal.end());
				current.resize(orignal.size());
				prevParent = current.back();
			}
			else
			{
				prevParent = head;
			}
		}

		static_cast<engine::GameObject>(prevParent).AddChild(child);
		if (copyTransform.GetChildCount())
		{
			orignal.emplace_back(copyTransform.GetID());
			current.emplace_back(child.GetID());
			prevParent = child;
		}
	}

}

}
