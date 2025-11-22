#include <GL/glew.h>
#include <GLFW/glfw3.h>  

#include "SceneFactory.h"
#include "DrawableObject.h"
#include "LightObject.h"
#include "Camera.h"
#include "Light.h"
#include "SpotLight.h"
#include "SpotLightTracker.h"
#include "ITransformComponent.h"
#include "ScaleTransform.h"
#include "RotateTransform.h"
#include "Texture.h"
#include "WindowManager.h"
#include "DynamicRotateTransform.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

SceneFactory::SceneFactory()
    : rng(std::random_device{}()), dist(0.0f, 1.0f)
{
}

float SceneFactory::randomFloat(float min, float max)
{
    //return min + dist(rng) * (max - min);
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

Scene* SceneFactory::createScene(int sceneID, float aspectRatio)
{
    switch (sceneID)
    {
    case 1:
        return createScene1(aspectRatio);
    case 2:
        return createScene2(aspectRatio);
    case 3:
        return createScene3(aspectRatio);
    case 4:
        return createScene4(aspectRatio);
    default:
        std::cerr << "Invalid scene ID: " << sceneID << std::endl;
        return nullptr;
    }
}

Scene* SceneFactory::createScene1(float aspectRatio)
{
    std::cout << "\nCreating Scene 1..." << std::endl;
    Scene* scene = new Scene();

    ShaderProgram* phongShader = scene->createShader(
        "shaders/phong_vertex.glsl",
        "shaders/phong_fragment.glsl"
    );

    ShaderProgram* constantShader = scene->createShader(
        "shaders/constant_vertex.glsl",
        "shaders/constant_fragment.glsl"
    );

    ShaderProgram* lambertShader = scene->createShader(
        "shaders/lambert_vertex.glsl",
        "shaders/lambert_fragment.glsl"
    );


    Texture* grassTexture = new Texture();
    if (!grassTexture->loadFromFile("texture/grass.png")) {
        std::cerr << "Failed to load grass texture!" << std::endl;
        delete grassTexture;
        grassTexture = nullptr;
    }

    Texture* woodTexture = new Texture();
    if (!woodTexture->loadFromFile("texture/wooden_fence.png")) {
        std::cerr << "Failed to load wooden fence texture!" << std::endl;
        delete woodTexture;
        woodTexture = nullptr;
    }

    Texture* shrekTexture = new Texture();
    if (!shrekTexture->loadFromFile("texture/shrek.png")) {
        std::cerr << "Failed to load shrekTexture fence texture!" << std::endl;
        delete shrekTexture;
        shrekTexture = nullptr;
    }

    Texture* fionaTexture = new Texture();
    if (!fionaTexture->loadFromFile("texture/fiona.png")) {
        std::cerr << "Failed to load fionaTexture fence texture!" << std::endl;
        delete fionaTexture;
        fionaTexture = nullptr;
    }

    Camera* camera = new Camera(
        glm::vec3(0.0f, 8.0f, 20.0f),
        glm::vec3(0.0f, 8.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        aspectRatio,
        0.1f,
        100.0f
    );
    scene->setCamera(camera);


    Light* moonlight = new Light(
        glm::vec3(0.0f, 50.0f, 0.0f),
        glm::vec3(0.05f, 0.05f, 0.08f),
        0.1f,
        1.0f, 0.007f, 0.0002f
    );

    Light* sunlight = new Light(
        glm::vec3(10.0f, 50.0f, 10.0f),
        glm::vec3(1.0f, 0.95f, 0.8f),
        2.0f,
        1.0f, 0.001f, 0.000001f
    );

    scene->addLight(sunlight);


    SpotLight* flashlight = new SpotLight(
        camera->getEye(),
        camera->getTarget(),
        glm::vec3(1.0f, 1.0f, 0.9f),
        10.0f,
        10.0f,
        20.0f,
        1.0f,
        0.022f,
        0.019f
    );

    scene->setSpotLight(flashlight);

    SpotLightTracker* tracker = new SpotLightTracker(flashlight);
    camera->attach(tracker);

    DrawableObject* teren = new DrawableObject();
    teren->setShader(lambertShader);
    if (teren->loadModelFromOBJ("models/teren.obj")) {
        teren->setTexture(grassTexture);
        teren->setObjectColor(glm::vec3(0.2f, 0.6f, 0.2f));
        teren->setShininess(32.0f);
        scene->addObject(teren);
    }
    else {
        std::cerr << "Failed to load teren.obj model!" << std::endl;
        delete teren;
    }

    DrawableObject* shrek = new DrawableObject(false);
    shrek->setShader(lambertShader);

    if (shrek->loadModelFromOBJ("models/shrek.obj")) {
        shrek->setTexture(shrekTexture);
        shrek->addStaticTransform(new TranslateTransform(glm::vec3(-9.0f, 0.0f, 0.0f)));
        shrek->addStaticTransform(new ScaleTransform(glm::vec3(2.0f, 2.0f, 2.0f)));
        scene->addObject(shrek);
    }

    DrawableObject* fiona = new DrawableObject(false);
    fiona->setShader(lambertShader);

    if (fiona->loadModelFromOBJ("models/fiona.obj")) {
        fiona->setTexture(fionaTexture);
        fiona->addStaticTransform(new TranslateTransform(glm::vec3(-5.0f, 0.0f, 0.0f)));
        fiona->addStaticTransform(new ScaleTransform(glm::vec3(2.0f, 2.0f, 2.0f)));
        fiona->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 60.0f));
        scene->addObject(fiona);
    }


    DrawableObject* cubeFront = new DrawableObject(false);
    cubeFront->setShader(lambertShader);
    if (cubeFront->loadModelFromText("models/square.txt")) {
        cubeFront->setTexture(woodTexture);
        cubeFront->setShininess(32.0f);
        cubeFront->addStaticTransform(new TranslateTransform(glm::vec3(0.0f, 2.0f, 0.0f)));
        cubeFront->addStaticTransform(new ScaleTransform(glm::vec3(2.0f, 2.0f, 2.0f)));
        scene->addObject(cubeFront);
    }

    DrawableObject* cubeRight = new DrawableObject(false);
    cubeRight->setShader(lambertShader);
    if (cubeRight->loadModelFromText("models/square.txt")) {
        cubeRight->setTexture(woodTexture);
        cubeRight->setShininess(32.0f);
        cubeRight->addStaticTransform(new TranslateTransform(glm::vec3(1.0f, 2.0f, 1.0f)));
        cubeRight->addStaticTransform(new RotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 270.0f));
        cubeRight->addStaticTransform(new ScaleTransform(glm::vec3(2.0f, 2.0f, 2.0f)));
        scene->addObject(cubeRight);
    }

    DrawableObject* cubeLeft = new DrawableObject(false);
    cubeLeft->setShader(lambertShader);
    if (cubeLeft->loadModelFromText("models/square.txt")) {
        cubeLeft->setTexture(woodTexture);
        cubeLeft->setShininess(32.0f);
        cubeLeft->addStaticTransform(new TranslateTransform(glm::vec3(-1.0f, 2.0f, 1.0f)));
        cubeLeft->addStaticTransform(new RotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f));
        cubeLeft->addStaticTransform(new ScaleTransform(glm::vec3(2.0f, 2.0f, 2.0f)));
        scene->addObject(cubeLeft);
    }

    DrawableObject* cubeBottom = new DrawableObject(false);
    cubeBottom->setShader(lambertShader);
    if (cubeBottom->loadModelFromText("models/square.txt")) {
        cubeBottom->setTexture(woodTexture);
        cubeBottom->setShininess(32.0f);
        cubeBottom->addStaticTransform(new TranslateTransform(glm::vec3(0.0f, 1.0f, 1.0f)));
        cubeBottom->addStaticTransform(new RotateTransform(glm::vec3(1.0f, 0.0f, 0.0f), 270.0f));
        cubeBottom->addStaticTransform(new ScaleTransform(glm::vec3(2.0f, 2.0f, 2.0f)));
        scene->addObject(cubeBottom);
    }


    DrawableObject* car = new DrawableObject(false);
    car->setShader(phongShader);
    if (car->loadModelFromOBJ("models/formula1.obj")) {
        car->setObjectColor(glm::vec3(0.8f, 0.2f, 0.4f));
        car->setShininess(64.0f);
        car->addStaticTransform(new TranslateTransform(glm::vec3(10.0f, 0.0f, 10.0f)));
        car->addStaticTransform(new ScaleTransform(glm::vec3(0.25f, 0.25f, 0.25f)));
        scene->addObject(car);
    }


    DrawableObject* tree = new DrawableObject(false);
    tree->setShader(phongShader);
    if (tree->loadModel("models/tree.h", "tree")) {
        tree->setObjectColor(glm::vec3(0.3f, 0.2f, 0.1f));
        tree->setShininess(96.0f);
        tree->addStaticTransform(new TranslateTransform(glm::vec3(5.0f, 0.0f, 5.0f)));
        scene->addObject(tree);
    }


    int gridSize = 3;
    float spacing = 8.0f;
    float territoryRadius = 3.0f;
    int fireflyCount = 0;

    std::cout << "\nAdding LightObject... (fireflies)" << std::endl;
    for (int x = 0; x < gridSize; x++) {
        for (int z = 0; z < gridSize; z++) {
            glm::vec3 center(
                (x - gridSize / 2) * spacing + (rand() % 100 - 50) / 50.0f,
                0.0f,
                (z - gridSize / 2) * spacing + (rand() % 100 - 50) / 50.0f
            );

            LightObject* firefly = new LightObject(
                center,
                territoryRadius,
                0.3f,
                2.5f,
                glm::vec3(1.0f, 1.0f, 0.3f),
                2.5f,
                1.0f, 0.22f, 0.20f
            );

            firefly->setShader(constantShader);

            if (firefly->loadModel("models/sphere.h", "sphere")) {
                firefly->setObjectColor(glm::vec3(1.0f, 1.0f, 0.3f));
                firefly->addStaticTransform(new ScaleTransform(glm::vec3(0.05f, 0.05f, 0.05f)));
                firefly->setSpeed(1.5f + (rand() % 100) / 100.0f);

                scene->addLightObject(firefly);
                fireflyCount++;
            }
            else {
                delete firefly;
            }
        }
    }

    std::cout << "Scene 1 created!" << std::endl;

    return scene;
}

Scene* SceneFactory::createScene2(float aspectRatio)
{
    std::cout << "\nCreating Scene 2..." << std::endl;

    Scene* scene = new Scene();

    ShaderProgram* constantShader = scene->createShader(
        "shaders/constant_vertex.glsl",
        "shaders/constant_fragment.glsl"
    );
    ShaderProgram* lambertShader = scene->createShader(
        "shaders/lambert_vertex.glsl",
        "shaders/lambert_fragment.glsl"
    );
    ShaderProgram* phongShader = scene->createShader(
        "shaders/phong_vertex.glsl",
        "shaders/phong_fragment.glsl"
    );
    ShaderProgram* blinnShader = scene->createShader(
        "shaders/blinn_vertex.glsl",
        "shaders/blinn_fragment.glsl"
    );

    Camera* camera = new Camera(
        glm::vec3(0.0f, 3.0f, 15.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        aspectRatio,
        0.1f,
        100.0f
    );
    scene->setCamera(camera);

    Light* light1 = new Light(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.5f,
        1.0f, 0.14f, 0.07f
    );

    Light* light2 = new Light(
        glm::vec3(-7.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        1.5f,
        1.0f, 0.22f, 0.00002f
    );

    Light* light3 = new Light(
        glm::vec3(7.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        1.5f,
        1.0f, 0.22f, 0.00002f
    );

    scene->addLight(light1);
    scene->addLight(light2);
    scene->addLight(light3);


    DrawableObject* sphere1 = new DrawableObject(false);
    sphere1->setShader(phongShader);

    if (sphere1->loadModelFromText("models/sphere.txt")) {
        sphere1->setObjectColor(glm::vec3(0.2f, 0.2f, 0.8f));
        sphere1->addStaticTransform(new TranslateTransform(glm::vec3(-3.0f, 0.0f, 0.0f)));
        sphere1->addStaticTransform(new ScaleTransform(glm::vec3(1.5f, 1.5f, 1.5f)));
        scene->addObject(sphere1);
    }
    else {
        delete sphere1;
    }

    DrawableObject* sphere2 = new DrawableObject(false);
    sphere2->setShader(phongShader);

    if (sphere2->loadModel("models/sphere.h", "sphere")) {
        sphere2->setObjectColor(glm::vec3(0.2f, 0.2f, 0.8f));
        sphere2->addStaticTransform(new TranslateTransform(glm::vec3(0.0f, 3.0f, 0.0f)));
        sphere2->addStaticTransform(new ScaleTransform(glm::vec3(1.5f, 1.5f, 1.5f)));
        scene->addObject(sphere2);
    }
    else {
        delete sphere2;
    }

    DrawableObject* sphere3 = new DrawableObject(false);
    sphere3->setShader(phongShader);

    if (sphere3->loadModel("models/sphere.h", "sphere")) {
        sphere3->setObjectColor(glm::vec3(0.2f, 0.2f, 0.8f));
        sphere3->setShininess(32.0f);
        sphere3->addStaticTransform(new TranslateTransform(glm::vec3(3.0f, 0.0f, 0.0f)));
        sphere3->addStaticTransform(new ScaleTransform(glm::vec3(1.5f, 1.5f, 1.5f)));
        scene->addObject(sphere3);
    }
    else {
        delete sphere3;
    }

    DrawableObject* sphere4 = new DrawableObject(false);
    sphere4->setShader(phongShader);

    if (sphere4->loadModel("models/sphere.h", "sphere")) {
        sphere4->setObjectColor(glm::vec3(0.2f, 0.2f, 0.8f));
        sphere4->setShininess(100.0f);
        sphere4->addStaticTransform(new TranslateTransform(glm::vec3(0.0f, -3.0f, 0.0f)));
        sphere4->addStaticTransform(new ScaleTransform(glm::vec3(1.5f, 1.5f, 1.5f)));
        scene->addObject(sphere4);
    }
    else {
        delete sphere4;
    }

    std::cout << "Scene 2 created" << std::endl;

    return scene;
}

Scene* SceneFactory::createScene3(float aspectRatio)
{
    std::cout << "\nCreating Scene 3" << std::endl;

    Scene* scene = new Scene();

    ShaderProgram* lambertShader = scene->createShader(
        "shaders/lambert_vertex.glsl",
        "shaders/lambert_fragment.glsl"
    );
    ShaderProgram* phongShader = scene->createShader(
        "shaders/phong_vertex.glsl",
        "shaders/phong_fragment.glsl"
    );

    ShaderProgram* constantShader = scene->createShader(
        "shaders/constant_vertex.glsl",
        "shaders/constant_fragment.glsl"
    );

    Camera* camera = new Camera(
        glm::vec3(0.0f, 5.0f, 20.0f),
        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        aspectRatio,
        0.1f,
        200.0f
    );
    scene->setCamera(camera);

    Light* sunlight = new Light(
        glm::vec3(10.0f, 50.0f, 10.0f),
        glm::vec3(1.0f, 0.95f, 0.8f),
        2.0f,
        1.0f, 0.001f, 0.000001f
    );

    Light* moonlight = new Light(
        glm::vec3(0.0f, 50.0f, 0.0f),
        glm::vec3(0.05f, 0.05f, 0.08f),
        0.1f,
        1.0f, 0.007f, 0.0002f
    );

    SpotLight* flashlight = new SpotLight(
        camera->getEye(),
        camera->getTarget(),
        glm::vec3(1.0f, 1.0f, 0.9f),
        10.0f,
        10.0f,
        20.0f,
        1.0f,
        0.022f,
        0.019f
    );

    scene->setSpotLight(flashlight);

    SpotLightTracker* tracker = new SpotLightTracker(flashlight);
    camera->attach(tracker);

    scene->addLight(moonlight);

    DrawableObject* ground = new DrawableObject(false);
    ground->setShader(lambertShader);

    if (ground->loadModelFromText("models/plain.txt")) {
        ground->setObjectColor(glm::vec3(0.2f, 0.6f, 0.2f));
        ground->addStaticTransform(new ScaleTransform(glm::vec3(40.0f, 1.0f, 40.0f)));
        ground->addStaticTransform(new TranslateTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
        scene->addObject(ground);
    }
    else {
        delete ground;
    }

    int treeCount = 0;
    int bushCount = 0;

    for (int i = 0; i < 50; i++) {
        DrawableObject* tree = new DrawableObject(false);
        tree->setShader(lambertShader);

        if (tree->loadModel("models/tree.h", "tree")) {
            tree->setObjectColor(glm::vec3(0.4f, 0.25f, 0.1f));
            tree->setShininess(16.0f);

            float xPos, zPos;
            do {
                xPos = randomFloat(-30.0f, 30.0f);
                zPos = randomFloat(-30.0f, 30.0f);
            } while (sqrt(xPos * xPos + zPos * zPos) < 3.0f);

            float scale = randomFloat(0.8f, 1.5f);

            float rotation = randomFloat(0.0f, 360.0f);

            tree->addStaticTransform(new TranslateTransform(glm::vec3(xPos, 0.0f, zPos)));
            tree->addStaticTransform(new ScaleTransform(glm::vec3(scale, scale, scale)));
            tree->addStaticTransform(new RotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotation)));

            scene->addObject(tree);
            treeCount++;
        }
        else {
            delete tree;
            std::cerr << "Failed to load tree model!" << std::endl;
        }
    }

    for (int i = 0; i < 50; i++) {
        DrawableObject* bush = new DrawableObject(false);
        bush->setShader(lambertShader);

        if (bush->loadModel("models/bushes.h", "bushes")) {
            bush->setObjectColor(glm::vec3(0.1f, 0.5f, 0.1f));


            float xPos, zPos;
            do {
                xPos = randomFloat(-30.0f, 30.0f);
                zPos = randomFloat(-30.0f, 30.0f);
            } while (sqrt(xPos * xPos + zPos * zPos) < 3.0f);

            float scale = randomFloat(0.5f, 1.0f);

            bush->addStaticTransform(new TranslateTransform(glm::vec3(xPos, 0.0f, zPos)));
            bush->addStaticTransform(new ScaleTransform(glm::vec3(scale, scale, scale)));

            scene->addObject(bush);
            bushCount++;
        }
        else {
            delete bush;
            std::cerr << "Failed to load bush model!" << std::endl;
        }
    }

    int gridWidth = 4;
    int gridHeight = 3;
    float spacing = 9.0f;
    float territoryRadius = 3.5f;

    int fireflyCount = 0;

    std::cout << "\nAdding LightObject... (fireflies)" << std::endl;
    for (int z = 0; z < gridHeight; z++) {
        for (int x = 0; x < gridWidth; x++) {
            float offsetX = (rand() % 200 - 100) / 100.0f;
            float offsetZ = (rand() % 200 - 100) / 100.0f;

            glm::vec3 center(
                (x - gridWidth / 2.0f + 0.5f) * spacing + offsetX,
                0.0f,
                (z - gridHeight / 2.0f + 0.5f) * spacing + offsetZ
            );

            float colorVariation = 0.2f + (rand() % 100) / 500.0f;
            glm::vec3 color(1.0f, 1.0f, colorVariation);

            LightObject* firefly = new LightObject(
                center,
                territoryRadius,
                0.3f,
                2.8f,
                color,
                2.0f + (rand() % 100) / 100.0f,
                1.0f, 0.22f, 0.20f
            );

            firefly->setShader(constantShader);


            if (firefly->loadModel("models/sphere.h", "sphere")) {
                firefly->setObjectColor(color);
                firefly->addStaticTransform(new ScaleTransform(glm::vec3(0.05f, 0.05f, 0.05f)));

                firefly->setSpeed(1.2f + (rand() % 150) / 100.0f);

                scene->addLightObject(firefly);
                fireflyCount++;
            }
            else {
                delete firefly;
            }
        }
    }

    std::cout << "Scene 3 created!" << std::endl;

    return scene;
}

Scene* SceneFactory::createScene4(float aspectRatio)
{
    std::cout << "\nCreating Scene 4..." << std::endl;

    Scene* scene = new Scene();

    ShaderProgram* constantShader = scene->createShader(
        "shaders/constant_vertex.glsl",
        "shaders/constant_fragment.glsl"
    );
    ShaderProgram* phongShader = scene->createShader(
        "shaders/phong_vertex.glsl",
        "shaders/phong_fragment.glsl"
    );
    ShaderProgram* blinnShader = scene->createShader(
        "shaders/blinn_vertex.glsl",
        "shaders/blinn_fragment.glsl"
    );
    ShaderProgram* lambertShader = scene->createShader(
        "shaders/lambert_vertex.glsl",
        "shaders/lambert_fragment.glsl"
    );

    Texture* earthTexture = new Texture();
    if (!earthTexture->loadFromFile("texture/earth.jpg")) {
        std::cerr << "Failed to load earthTexture!" << std::endl;
        delete earthTexture;
        earthTexture = nullptr;
    }

    Camera* camera = new Camera(
        glm::vec3(0.0f, 15.0f, 30.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        aspectRatio,
        0.1f,
        500.0f
    );
    scene->setCamera(camera);

    Light* sunLight = new Light(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.9f, 0.6f),
        3.0f
    );
    scene->addLight(sunLight);

    DrawableObject* sun = new DrawableObject(false);
    sun->setShader(constantShader);

    if (sun->loadModel("models/sphere.h", "sphere")) {
        sun->setObjectColor(glm::vec3(1.0f, 0.9f, 0.2f));
        sun->addStaticTransform(new ScaleTransform(glm::vec3(3.0f)));
        scene->addObject(sun);
        std::cout << "Sun loaded!" << std::endl;
    }
    else {
        delete sun;
    }

    DrawableObject* earth = new DrawableObject(true);
    earth->setShader(lambertShader);

    if (earth->loadModel("models/sphere.h", "sphere")) {
        earth->setObjectColor(glm::vec3(0.5f, 0.5f, 1.0f));
        earth->setShininess(32.0f);

        earth->addStaticTransform(new ScaleTransform(glm::vec3(1.5f)));

        earth->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 50.0f));

        earth->addDynamicTransform(new TranslateTransform(glm::vec3(12.0f, 0.0f, 0.0f)));

        earth->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 5.0f));

        scene->addObject(earth);
        std::cout << "Earth loaded!" << std::endl;
    }
    else {
        delete earth;
        earth = nullptr;
    }

    if (earth != nullptr) {
        DrawableObject* moon = new DrawableObject(true);
        moon->setShader(blinnShader);

        if (moon->loadModel("models/sphere.h", "sphere")) {
            moon->setObjectColor(glm::vec3(0.9f, 0.9f, 0.9f));
            moon->setShininess(16.0f);

            moon->setParent(earth);

            moon->addStaticTransform(new ScaleTransform(glm::vec3(0.5f)));
            // around earth
            moon->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f));

            moon->addDynamicTransform(new TranslateTransform(glm::vec3(3.0f, 0.0f, 0.0f)));
            // around herself
            moon->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f));

            scene->addObject(moon);
            std::cout << "Moon loaded!" << std::endl;
        }
        else {
            delete moon;
        }
    }

    std::cout << "Scene 4 created!" << std::endl;

    return scene;
}