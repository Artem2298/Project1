#pragma once
#include "Scene.h"
#include <random>

class SceneFactory
{
private:
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;

    float randomFloat(float min, float max);

    Scene* createScene1(float aspectRatio);
    Scene* createScene2(float aspectRatio);
    Scene* createScene3(float aspectRatio);
    Scene* createScene4(float aspectRatio);

public:
    SceneFactory();
    ~SceneFactory() = default;

    Scene* createScene(int sceneID, float aspectRatio);
};