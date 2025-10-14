#include "SceneManager.h"
#include <iostream>

SceneManager::SceneManager()
    : currentScene(nullptr), currentSceneID(-1)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::addScene(int sceneID, Scene* scene)
{
    if (scene == nullptr)
    {
        std::cerr << "ERROR: Cannot add nullptr scene\n";
        return;
    }

    scenes[sceneID] = std::unique_ptr<Scene>(scene);

    if (currentScene == nullptr)
    {
        currentScene = scene;
        currentSceneID = sceneID;
    }

    std::cout << "Scene " << sceneID << " added. Total scenes: " << scenes.size() << "\n";
}

void SceneManager::switchScene(int sceneID)
{
    auto it = scenes.find(sceneID);

    if (it == scenes.end())
    {
        std::cerr << "ERROR: Scene " << sceneID << " not found\n";
        return;
    }

    currentScene = it->second.get();
    currentSceneID = sceneID;

    std::cout << "Switched to scene " << sceneID << "\n";
}