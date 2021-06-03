//#include "pch.h"
#include "Editor.h"

#include "../ImGui/imgui.h"
#include <filesystem>

#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include "../../vendor/ImGui/imgui_internal.h"

#include "Engine/Core/Input.h"

/* static vars */
static testclass rootnode;

Editor::Editor(const std::string& root) :m_rootPath{ root },m_currentPath{root}
{
	testList.reserve(50);

	m_hotkeymapping[KEY_ACTIONS::RENAME_ITEM] = 59;//f2
	m_hotkeymapping[KEY_ACTIONS::DELETE_ITEM] = 76;//del
	m_hotkeymapping[KEY_ACTIONS::HIDE_INSPECTOR] = 30;//1
	m_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTHIRECHY] = 31;//2
	m_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTFOLDERVIEW] = 32;//3
	m_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTVIEW] = 33;//4

	m_activeFlagGUI |= static_cast<int>(GUIACTIVE_FLAGS::INSPECTOR_ACTIVE);
	m_activeFlagGUI |= static_cast<int>(GUIACTIVE_FLAGS::PROJECTHIRECHY_ACTIVE);
	m_activeFlagGUI |= static_cast<int>(GUIACTIVE_FLAGS::PROJECTFOLDERVIEW_ACTIVE);
	m_activeFlagGUI |= static_cast<int>(GUIACTIVE_FLAGS::PROJECTVIEW_ACTIVE);

}

Editor::~Editor()
{
}

void Editor::HotKeysUpdate()
{
	/*if(engine::Input::IsKeyPressed(engine::key::W))
	{
		SetGUIInactive(GUIACTIVE_FLAGS::INSPECTOR_ACTIVE);
	}*/

	if (ImGui::IsKeyPressed(m_hotkeymapping[KEY_ACTIONS::HIDE_INSPECTOR]))
	{
		SetGUIInactive(GUIACTIVE_FLAGS::INSPECTOR_ACTIVE);
	}
	else if (ImGui::IsKeyPressed(m_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTHIRECHY]))
	{
		SetGUIInactive(GUIACTIVE_FLAGS::PROJECTHIRECHY_ACTIVE);
	}
	else if (ImGui::IsKeyPressed(m_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTFOLDERVIEW]))
	{
		SetGUIInactive(GUIACTIVE_FLAGS::PROJECTFOLDERVIEW_ACTIVE);
	}
	else if (ImGui::IsKeyPressed(m_hotkeymapping[KEY_ACTIONS::HIDE_PROJECTVIEW]))
	{
		SetGUIInactive(GUIACTIVE_FLAGS::PROJECTVIEW_ACTIVE);
	}
}
void Editor::UpdateUI()
{
}


void Editor::ShowObject(testclass& tc)
{
	bool activated = false;
	ImGuiTreeNodeFlags flag = 0;


	if (m_focused == &tc)
	{
		flag = ImGuiTreeNodeFlags_Selected;

		if (ImGui::IsKeyPressed(m_hotkeymapping[KEY_ACTIONS::RENAME_ITEM]))
			m_editing = true;

		if (m_dragging)
		{
			flag |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
			m_dragging = !ImGui::IsMouseReleased(ImGuiMouseButton_Left);
		}

		if (m_editing)
		{
			//if editing item ( draw a text box above )
			static char s_Buffer[100];

			ImGui::PushID(tc.uid);
			if (ImGui::InputText("rename", s_Buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
			{
				tc.name = s_Buffer;
				s_Buffer[0] = '\0';//empty the character buffer
			}
			if (!ImGui::IsItemFocused())
				ImGui::SetKeyboardFocusHere();
			if (ImGui::IsItemDeactivated())//if clicked else where the textbox will dissapear
				m_editing = false;

			ImGui::PopID();
		}
	}
	flag |= (tc.childs.size()) ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	
	ImGui::PushID(tc.uid);
	activated = ImGui::TreeNodeEx((tc.name).c_str() , flag);
	ImGui::PopID();

	if (ImGui::IsItemClicked())
		m_focused = &tc;
	
	//drop
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERACHY_OBJ");
		if (payload)
		{
			m_dragging = false;
			m_focused->SetParent(&tc);
		}
		ImGui::EndDragDropTarget();
	}
	//drag
	if (m_focused && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAutoExpirePayload))
	{
		// Set payload to carry the index of our item (could be anything)
		m_dragging = true;
		ImGui::SetDragDropPayload("HIERACHY_OBJ", nullptr, 0);
		ImGui::Text("%s", m_focused->name.c_str());
		ImGui::EndDragDropSource();
	}
	//creating childs(recurse)
	if (activated && tc.childs.size() && !(flag & ImGuiTreeNodeFlags_NoTreePushOnOpen))
	{
		for (testclass* obj : tc.childs)
		{
			ShowObject(*obj);
		}
		ImGui::TreePop();
	}
}

//all the filepaths
void Editor::ProjectFile(const std::string& path, std::string& selected_dir)
{
	static std::vector<char> layer;//the layer selected
	static std::vector<char> curr;//the current layer
	static std::string selected_itemname = "";

	ImGuiTreeNodeFlags flag = 0;

	//this is to get a img to draw
	ImGuiIO& io = ImGui::GetIO();
	ImFontAtlas* atlas = io.Fonts;

	char counter = 0;
	bool enable = false;
	for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_directory())//if item is a folder use the folder img
		{
			flag = ImGuiTreeNodeFlags_OpenOnArrow;
			ImGui::Image(atlas->TexID, { 10,10 });
		}
		else//use some img
		{
			flag = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			ImGui::Image(atlas->TexID, { 20,10 });
		}
		ImGui::SameLine();

		if (layer == curr)
		{
			if (entry.path().filename().generic_u8string() == selected_itemname)
				flag |= ImGuiTreeNodeFlags_Selected;
		}

		enable = ImGui::TreeNodeEx(entry.path().filename().generic_u8string().c_str(), flag);

		if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
		{
			if (entry.is_directory())//for the previde module
				selected_dir = entry.path().generic_u8string();
			//to mark the item that is focused
			selected_itemname = entry.path().filename().generic_u8string();
			layer = curr;
		}

		if (enable && entry.is_directory())
		{
			ImGui::PushID(counter);
			curr.emplace_back(counter);
			ProjectFile(entry.path().u8string(), selected_dir);
			curr.pop_back();
			ImGui::PopID();

			ImGui::TreePop();//pop the tree nodes
		}
		++counter;
	}
}

//show a clickable directory path and modify it when clicked
void Editor::PathDir(std::filesystem::path& entry, std::string& path)
{
	static int count = 0;
	bool selected = false;
	if (entry.has_parent_path() && count < 5)
	{
		++count;
		PathDir(entry.parent_path(), path);
		//it is currently hardcoded
		ImGui::Selectable(entry.filename().generic_u8string().c_str(), &selected, 0, { 7.0f * entry.filename().generic_u8string().size(),13 });
		ImGui::SameLine();
		ImGui::Bullet();
	}
	if (count)//exception for the last object
	{
		--count;
		ImGui::SameLine();
	}
	if (selected)
		path = entry.generic_u8string().c_str();
}
//enlarged view of the projectfile
void Editor::PreviewFolder()
{
	//remove this once integrated to the rendering system
	ImGuiIO& io = ImGui::GetIO(); // use for scrolling
	ImFontAtlas* atlas = io.Fonts;

	//padding
	static const float max_padding = 20;
	static const float max_imgsize = 128;

	static float padding = max_padding;
	static float imgsize = max_imgsize;

	static int size_multiplier = 1;

	//local variables
	std::filesystem::directory_iterator dir_iter = std::filesystem::directory_iterator(m_currentPath);

	//when scrolled
	float scroll_count = io.MouseWheel;
	if (io.KeyCtrl && (scroll_count))
	{
		scroll_count = abs(scroll_count) > 1 ? scroll_count * 0.5f : scroll_count / abs(scroll_count);
		size_multiplier -= scroll_count;
		size_multiplier = size_multiplier < 1 ? 1 : size_multiplier;//min
		size_multiplier = size_multiplier > 7 ? 7 : size_multiplier;//max
		padding = max_padding / size_multiplier;//determin the padding when scrolled
		imgsize = max_imgsize / size_multiplier;
	}
	//show directory
	ImGui::BeginChild("preview_directory", { ImGui::GetContentRegionAvailWidth(),30 }, true);
	PathDir(std::filesystem::path(m_currentPath), m_currentPath);
	ImGui::EndChild();

	//table calculation
	float row = ImGui::GetContentRegionAvailWidth() / (padding + imgsize);
	if (ImGui::BeginTable("preview_table", static_cast<int>(row)) == false)//when changing tabs this will be set to false
		return;
	ImGui::TableNextColumn();//push 1 column first
	for (std::filesystem::directory_entry entry : dir_iter)
	{
		ImGui::BeginGroup();//start
		//change this would be changed once rendering is integrated
		if (entry.path().filename().has_extension() == false)
			ImGui::ImageButton(atlas->TexID, { imgsize, imgsize }, { 0,0 }, { 0.125,1 });
		else
			ImGui::ImageButton(atlas->TexID, { imgsize, imgsize });
		//text of the filename
		ImGui::TextWrapped(entry.path().filename().generic_u8string().c_str());
		ImGui::EndGroup();//end
		ImGui::TableNextColumn();//item done

		//interactions of item
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
		{
			if (entry.is_directory())
			{
				m_currentPath = entry.path().generic_u8string().c_str();
				ImGui::EndTable();
				return;
			}
			//can open c# file here
			else if (entry.path().has_extension())
			{
				std::string a = entry.path().generic_u8string().c_str();
				std::system(a.substr(2).c_str());//substr can be removed one we have a proper filepath
			}
		}
	}
	ImGui::EndTable();
	//drag and drop interaction for prefab
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERACHY_OBJ");
		if (payload)
		{

			std::ofstream stream("prefab");
			rapidjson::OStreamWrapper osw(stream);
			rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
			writer.StartObject();
			SaveHirechy(*m_focused, writer);
			writer.EndObject();
		}
		ImGui::EndDragDropTarget();
	}
}
void Editor::SaveObject(testclass& tc, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
{
	std::vector<rttr::property> list = rttr::type::get<testclass>().get_properties();

	writer.Key(std::to_string(tc.uid).c_str());
	writer.StartArray();

	writer.Key(tc.get_type().get_name().c_str());
	writer.StartArray();//start component 1
	for (int j = 0; j < list.size(); ++j)
	{
		if (list[j].get_type() == rttr::type::get<int>())
		{
			writer.Int(list[j].get_value(tc).to_int());
		}
		else if (list[j].get_type() == rttr::type::get<std::string>())
		{
			writer.String(list[j].get_value(tc).to_string().c_str());
		}
	}
	writer.EndArray();//end component 1
	writer.EndArray();

}
void Editor::SaveHirechy(testclass& tc, rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer)
{
	SaveObject(tc, writer);
	for (int i = 0; i < tc.childs.size(); ++i)
	{
		SaveHirechy(*tc.childs[i], writer);
	}
}
void Editor::SaveData()
{
	std::vector<rttr::property> list = rttr::type::get<testclass>().get_properties();
	std::ofstream stream("inspector.json");
	rapidjson::OStreamWrapper osw(stream);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
	writer.StartObject();
	for (int i = 0; i < testList.size(); ++i)
	{
		writer.Key(std::to_string(testList[i].uid).c_str());
		writer.StartArray();

		writer.Key(testList[i].get_type().get_name().c_str());
		writer.StartArray();//start component 1
		for (int j = 0; j < list.size(); ++j)
		{
			if (list[j].get_type() == rttr::type::get<int>())
			{
				writer.Int(list[j].get_value(testList[i]).to_int());
			}
			else if (list[j].get_type() == rttr::type::get<std::string>())
			{
				writer.String(list[j].get_value(testList[i]).to_string().c_str());
			}

		}
		writer.EndArray();//end component 1
		writer.EndArray();
	}
	writer.EndObject();
}
void Editor::LoadData(const char* dir)
{
	std::ifstream ifs(dir);
	rapidjson::Document doc;
	rapidjson::IStreamWrapper isw(ifs);
	doc.ParseStream(isw);
	std::vector<rttr::property> list = rttr::type::get<testclass>().get_properties();
	int counter = 0;
	for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
	{
		std::cout << (*it).name.GetString() << std::endl;
		rapidjson::GenericArray arr = it->value.GetArray();
		for (int i = 0; i < arr.Size(); ++i)
		{
			if (rttr::type::get_by_name(arr[i].GetString()).is_valid())
			{
				++i;
				rapidjson::GenericArray member = arr[i].GetArray();
				testList[counter].CreateTestClass(member[0].GetInt(), member[1].GetInt(), member[2].GetInt(), member[3].GetString());
			}
		}
		++counter;
	}
}
void Editor::ReadData()
{
	if (!m_focused)
		return;
	rttr::type t = m_focused->get_type();
	auto types = t.get_properties();

	ImGui::Begin("inspector");

	{
		ImGui::Text("Name :  %s", m_focused->name.c_str());
		ImGui::BeginChild("child", { 200,200 }, true);
		for (auto element : types)
		{
			if (element.get_type().get_id() == rttr::type::get<int>().get_id())
			{
				int value = element.get_value(m_focused).get_value<int>();
				ImGui::SliderInt(element.get_name().c_str(), &value, 0, 10);
				element.set_value(m_focused, value);
			}
		}
		ImGui::EndChild();

		ImGui::BeginChild("itemcount", { 200,200 }, true);

		ImGui::Text("Child Count :  %d", m_focused->childs.size());

		ImGui::EndChild();
	}
	ImGui::End();
}
void Editor::HierarchyPopUp()
{
	if (ImGui::MenuItem("New Object")) 
	{
		testclass tc{ 100 };
		tc.name = "new gameobject";
		testList.push_back(tc);
		testList[testList.size() - 1].SetParent(&rootnode);
	}
}
void Editor::FileWindowPopUp()
{
}
void Editor::TestFunction()
{
	//main banner
	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	if(m_activeFlagGUI & static_cast<int>(GUIACTIVE_FLAGS::INSPECTOR_ACTIVE))
	{
		ImGui::SetNextWindowSizeConstraints({ 350,350 }, { 1280,1080 });
		ReadData();
	}

	if (m_activeFlagGUI & static_cast<int>(GUIACTIVE_FLAGS::PROJECTHIRECHY_ACTIVE))
	{
		ImGui::SetNextWindowSizeConstraints({ 350,350 }, { 1280,1080 });
		ImGui::Begin("Hierarchy");
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
			ImGui::OpenPopup("hierarchy_PopUp");

		if (ImGui::BeginPopup("hierarchy_PopUp"))
		{
			HierarchyPopUp();
			ImGui::EndPopup();
		}
		for(testclass* obj : rootnode.childs)
			ShowObject(*obj);
		ImGui::End();
	}


	if (m_activeFlagGUI & static_cast<int>(GUIACTIVE_FLAGS::PROJECTVIEW_ACTIVE))
	{
		ImGui::SetNextWindowSizeConstraints({ 200,200 }, { 1280,1080 });
		ImGui::Begin("Project Dir");
		ProjectFile("./", m_currentPath);
		ImGui::End();
	}

	if (m_activeFlagGUI & static_cast<int>(GUIACTIVE_FLAGS::PROJECTFOLDERVIEW_ACTIVE))
	{
		//TODO:: change the max value to get from system
		ImGui::SetNextWindowSizeConstraints({ 350,350 }, { 1280,1080 });
		ImGui::Begin("Project Folder");
		PreviewFolder();
		ImGui::End();
	}

	HotKeysUpdate();
}
