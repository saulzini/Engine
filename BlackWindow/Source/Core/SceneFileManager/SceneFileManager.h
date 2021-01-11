#include "json/json.h"
#include <iostream>
#include <fstream>
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
class SceneFileManager
{
public:
    SceneFileManager(){}
    ~SceneFileManager(){}

    static void ExportFile(const char *name, const Json::Value &jsonNode){
        Json::StyledStreamWriter writer;
        std::ofstream outputFile(name);
        writer.write(outputFile, jsonNode);
        App->editor->consoleWindow->AddLog("Scene saved");
    }

	static void LoadFromFile(const char *name){
        Json::Value root;
        std::ifstream file(name);
        if(file.fail()){
            App->editor->consoleWindow->AddLog("Scene file not exists");
            return;
        }
        file >> root;
        App->scene->Load(root);
        App->editor->consoleWindow->AddLog("Scene loaded");
    }

};
