#pragma once
#include "Module.h"
#include <vector>
#include "UIWindow/DefaultImGuiWindow.h"
struct ImGuiIO;
class ConsoleWindow;
class ConfigurationWindow;
class PreviewWindow;
class HierarchyWindow;
class ModuleEditor : public Module
{

public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float deltaTime);
	update_status Update(float deltaTime);
	update_status PostUpdate(float deltaTime);
	bool CleanUp();

	void addWindow(DefaultImGuiWindow *window); //method for adding multiple windows

	ConsoleWindow *consoleWindow;
	ConfigurationWindow *configurationWindow;
	PreviewWindow *previewWindow;
	HierarchyWindow *hierarchyWindow;

private:
	std::vector<DefaultImGuiWindow *> windows;
	void ShowDockSpace(bool *pOpen);
	void DrawMenu();
	void SetStyle(const ImGuiIO io);
	void UpdateWindows();
	bool showDemoWindow;
	bool showDock;
	update_status appStatus;
};