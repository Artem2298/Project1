#pragma once
#include <unordered_map>
#include <memory>
#include "Scene.h"

class SceneManager
{
private:
    std::unordered_map<int, std::unique_ptr<Scene>> scenes;
    Scene* currentScene;
    int currentSceneID;

public:
    SceneManager();
    ~SceneManager();

    void addScene(int sceneID, Scene* scene);
    void switchScene(int sceneID);
    Scene* getCurrentScene() const { return currentScene; }
    int getCurrentSceneID() const { return currentSceneID; }
};