#pragma once
#include "ActionBehaviour.h" //base class
#include "Engine/Scripting/Scripting.h"
#include "Engine/Scripting/ScriptInfo.h" // script field info
#include "Engine/ECS/ECS_Utility.h" // entity
#include "Utility/Hash.h" // data type

#include "Engine/Prefab/EditorComponent.h"

class ScriptActionStack : public ActionBehaviour
{
public:
	ScriptActionStack(const std::string& msg, engine::ScriptClassInfo& script_reference ,engine::Entity object, engine::ScriptFieldInfo& undo, engine::ScriptFieldInfo& redo, bool is_dirty)
		:ActionBehaviour{ msg },
		m_object{ object }, m_script_reference{script_reference},
		m_undoData{ undo }, m_redoData{ redo },
		m_dirty{ is_dirty }
	{
	}
	void undo()
	{
		engine::GameObject& go = static_cast<engine::GameObject>(m_object);
		engine::Scripting& scriptcomponent = go.GetComponent<engine::Scripting>();
		engine::ScriptFieldInfo* info = scriptcomponent.GetScriptInfo(m_script_reference)->FindFieldInfo(m_undoData.name);
		*info = m_undoData;
		go.GetComponent<engine::EditorComponent>().SetPrefabDirty(m_dirty);
	}
	void redo()
	{
		engine::GameObject& go = static_cast<engine::GameObject>(m_object);
		engine::Scripting& scriptcomponent = go.GetComponent<engine::Scripting>();
		engine::ScriptFieldInfo* info = scriptcomponent.GetScriptInfo(m_script_reference)->FindFieldInfo(m_redoData.name);
		*info = m_redoData;
		go.GetComponent<engine::EditorComponent>().SetPrefabDirty(true);
	}

private:
	engine::Entity m_object;
	engine::ScriptClassInfo m_script_reference;
	engine::ScriptFieldInfo m_undoData;
	engine::ScriptFieldInfo m_redoData;
	bool m_dirty;
};

