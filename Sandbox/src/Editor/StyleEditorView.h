#pragma once
#include <imgui.h>
#include <string>
class StyleEditorView
{
public:
	StyleEditorView();
	~StyleEditorView();
	void Show();
private:
	void MenuBar();
	void SaveStyle();
	void LoadStyle();
	std::string name;
	ImGuiStyle ref;
	char namebuffer[100];
};

