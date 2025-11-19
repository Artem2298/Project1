#pragma once
#include "SceneManager.h"

class Renderer
{
private:
    SceneManager* sceneManager;

public:
    Renderer(SceneManager* sceneManager);
    ~Renderer();

    void render();
    void clear();
    void setViewport(int width, int height);
};