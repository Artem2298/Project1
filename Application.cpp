#include "Application.h"
#include "TranslateTransform.h"
#include "RotateTransform.h"
#include "ScaleTransform.h"
#include "DynamicRotateTransform.h"
#include "Camera.h"
#include "Light.h"
#include "DrawableObject.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <glm/gtc/constants.hpp>
#include "SpotLight.h"
#include "SpotLightTracker.h"

Application* Application::s_instance = nullptr;

Application::Application(int width, int height, const char* title)
    : window(nullptr),
    windowWidth(width),
    windowHeight(height),
    windowTitle(title),
    isRunning(false),
    lastFrameTime(0.0),
    lastMouseX(0.0),
    lastMouseY(0.0),
    rightMousePressed(false)
{
    s_instance = this;
    srand(static_cast<unsigned int>(time(nullptr)));
}

Application::~Application()
{
    shutdown();
}

void Application::error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error [%d]: %s\n", error, description);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        Application* app = s_instance;

        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        else if (key == GLFW_KEY_1) {
            app->sceneManager.switchScene(1);
            std::cout << "Switched to Scene 1: Triangle Test" << std::endl;
        }
        else if (key == GLFW_KEY_2) {
            app->sceneManager.switchScene(2);
            std::cout << "Switched to Scene 2: Four Spheres (Lighting Models)" << std::endl;
        }
        else if (key == GLFW_KEY_3) {
            app->sceneManager.switchScene(3);
            std::cout << "Switched to Scene 3: Forest" << std::endl;
        }
        else if (key == GLFW_KEY_4) {
            app->sceneManager.switchScene(4);
            std::cout << "Switched to Scene 4: Solar System" << std::endl;
        }
        else if (key == GLFW_KEY_F) {
            Scene* currentScene = app->sceneManager.getCurrentScene();
            if (currentScene) {
                SpotLight* flashlight = currentScene->getSpotLight();
                if (flashlight) {
                    bool currentState = flashlight->isEnabled();
                    flashlight->setEnabled(!currentState);

                    std::cout << "\n🔦 Flashlight: "
                        << (flashlight->isEnabled() ? "ON ✅" : "OFF ❌")
                        << std::endl;
                }
                else {
                    std::cout << "⚠️  No flashlight in current scene!" << std::endl;
                }
            }
        }
    }
}

void Application::processInput(float deltaTime)
{

    Scene* currentScene = sceneManager.getCurrentScene();

    if (!currentScene) {
        return;
    }

    Camera* camera = currentScene->getCamera();

    if (!camera) {
        std::cerr << "No camera in current scene!" << std::endl;
        return;
    }

    float cameraSpeed = 5.0f * deltaTime;

    glm::vec3 eye = camera->getEye();
    glm::vec3 forward = camera->getTarget();
    glm::vec3 up = camera->getUp();
    glm::vec3 right = glm::normalize(glm::cross(forward, up));

    bool cameraMoved = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        eye += forward * cameraSpeed;
        cameraMoved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        eye -= forward * cameraSpeed;
        cameraMoved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        eye -= right * cameraSpeed;
        cameraMoved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        eye += right * cameraSpeed;
        cameraMoved = true;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        eye += up * cameraSpeed;
        cameraMoved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        eye -= up * cameraSpeed;
        cameraMoved = true;
    }

    if (cameraMoved) {
        camera->setPosition(eye);
    }
}

void Application::window_size_callback(GLFWwindow* window, int width, int height)
{
    if (s_instance)
    {
        s_instance->windowWidth = width;
        s_instance->windowHeight = height;
        glViewport(0, 0, width, height);

        Scene* currentScene = s_instance->getSceneManager().getCurrentScene();
        if (currentScene)
        {
            float aspect = width / (float)height;
            glm::mat4 projection = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
            currentScene->setProjectionMatrix(projection);
        }
    }
}

bool Application::initGLFW()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: Failed to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "ERROR: Failed to create GLFW window\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    std::cout << "GLFW initialized successfully\n";
    return true;
}

bool Application::initGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        fprintf(stderr, "ERROR: GLEW initialization failed: %s\n", glewGetErrorString(err));
        return false;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    std::cout << "GLEW initialized successfully\n";
    return true;
}

void Application::printSystemInfo()
{
    printf("=== System Information ===\n");
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLEW Version: %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("==========================\n");
}

void Application::setupScenes()
{
    std::cout << "Setting up scenes..." << std::endl;

    createScene1();
    createScene2();
    createScene3();
    createScene4();

    sceneManager.switchScene(1);

    std::cout << "All scenes created!" << std::endl;
    std::cout << "Press 1-4 to switch between scenes" << std::endl;
}

void Application::createScene1()
{
    Scene* scene = new Scene();

    ShaderProgram* phongShader = scene->createShader(
        "shaders/phong_vertex.glsl",
        "shaders/phong_fragment.glsl"
    );

    ShaderProgram* constantShader = scene->createShader(
        "shaders/constant_vertex.glsl",
        "shaders/constant_fragment.glsl"
    );

    Camera* camera = new Camera(
        glm::vec3(0.0f, 8.0f, 20.0f),
        glm::vec3(0.0f, 8.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        (float)windowWidth / windowHeight,
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

    scene->addLight(moonlight);

    SpotLight* flashlight = new SpotLight(
        camera->getEye(),                 // Позиция камеры
        camera->getTarget(),              // Направление камеры
        glm::vec3(1.0f, 1.0f, 0.9f),     // Теплый белый свет
        10.0f,                            // Яркий!
        10.0f,                            // Внутренний угол
        20.0f,                            // Внешний угол
        1.0f,                             // constant
        0.022f,                            // linear
        0.019f                            // quadratic
    );

    scene->setSpotLight(flashlight);

    SpotLightTracker* tracker = new SpotLightTracker(flashlight);
    camera->attach(tracker);

    DrawableObject* ground = new DrawableObject(false);
    ground->setShader(phongShader);

    if(ground->loadModelFromText("models/plain.txt")) {
        ground->setObjectColor(glm::vec3(0.15f, 0.25f, 0.15f));
        ground->addStaticTransform(new ScaleTransform(glm::vec3(30.0f, 1.0f, 30.0f)));
        scene->addObject(ground);
    }

    DrawableObject* car = new DrawableObject(false);
    car->setShader(phongShader);

    if (car->loadModelFromOBJ("models/formula1.obj")) {
        car->setObjectColor(glm::vec3(0.8f, 0.2f, 0.4f));
        car->setShininess(64.0f);
        car->addStaticTransform(new ScaleTransform(glm::vec3(0.25f, 0.25f, 0.25f)));
        car->addStaticTransform(new TranslateTransform(glm::vec3(30.0f, 0.0f, 30.0f)));
        scene->addObject(car);
    }

    DrawableObject* tree = new DrawableObject(false);
    tree->setShader(phongShader);

    if (tree->loadModel("models/tree.h", "tree")) {
        tree->setObjectColor(glm::vec3(0.3f, 0.2f, 0.1f));
        tree->setShininess(96.0f);
        tree->addStaticTransform(new TranslateTransform(glm::vec3(0.0f, 0.0f, 0.0f)));
        scene->addObject(tree);
    }

    int gridSize = 3;
    float spacing = 8.0f;
    float territoryRadius = 3.0f;

    int fireflyCount = 0;

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

    sceneManager.addScene(1, scene);
}

void Application::createScene2()
{
    std::cout << "Creating Scene 3b: Four Spheres..." << std::endl;

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
        (float)windowWidth / windowHeight,
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
        std::cout << "Sphere 1 (Constant) loaded!" << std::endl;
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
        std::cout << "Sphere 2 (Lambert) loaded!" << std::endl;
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
        std::cout << "Sphere 3 (Phong) loaded!" << std::endl;
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
        std::cout << "Sphere 4 (Blinn-Phong) loaded!" << std::endl;
    }
    else {
        delete sphere4;
    }

    sceneManager.addScene(2, scene);
    std::cout << "Scene 3b created: 4 spheres with different shading models!" << std::endl;
}

void Application::createScene3()
{
    std::cout << "Creating Scene 3c: Forest..." << std::endl;

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
        (float)windowWidth / windowHeight,
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
    int gridHeight = 5;
    float spacing = 9.0f;
    float territoryRadius = 3.5f;

    int fireflyCount = 0;

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

    sceneManager.addScene(3, scene);
    std::cout << "Scene 3c created!" << std::endl;
}

void Application::createScene4()
{
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

    Camera* camera = new Camera(
        glm::vec3(0.0f, 15.0f, 30.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        (float)windowWidth / windowHeight,
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
        sun->addStaticTransform(new ScaleTransform(glm::vec3(3.0f, 3.0f, 3.0f)));
        scene->addObject(sun);
        std::cout << "Sun loaded (from cache)!" << std::endl;
    }
    else {
        delete sun;
    }

    DrawableObject* earth = new DrawableObject(true);
    earth->setShader(phongShader);

    if (earth->loadModel("models/sphere.h", "sphere")) {
        earth->setObjectColor(glm::vec3(0.2f, 0.4f, 0.8f));
        earth->setShininess(32.0f);

        earth->addStaticTransform(new ScaleTransform(glm::vec3(1.5f, 1.5f, 1.5f)));

        earth->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 20.0f));
        earth->addDynamicTransform(new TranslateTransform(glm::vec3(12.0f, 0.0f, 0.0f)));
        earth->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 2.0f));

        scene->addObject(earth);
        std::cout << "Earth loaded (from cache)!" << std::endl;
    }
    else {
        delete earth;
    }

    DrawableObject* moon = new DrawableObject(true);
    moon->setShader(blinnShader);

    if (moon->loadModel("models/sphere.h", "sphere")) {
        moon->setObjectColor(glm::vec3(0.7f, 0.7f, 0.7f));
        moon->setShininess(16.0f);

        moon->addStaticTransform(new ScaleTransform(glm::vec3(0.5f, 0.5f, 0.5f)));

        moon->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 0.4f));
        moon->addDynamicTransform(new TranslateTransform(glm::vec3(3.0f, 0.0f, 0.0f)));
        moon->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 20.0f));
        moon->addDynamicTransform(new TranslateTransform(glm::vec3(12.0f, 0.0f, 0.0f)));
        moon->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 2.0f));

        scene->addObject(moon);
        std::cout << "Moon loaded (from cache)!" << std::endl;
    }
    else {
        delete moon;
    }

    sceneManager.addScene(4, scene);
    std::cout << "Scene 3d created: Solar System!" << std::endl;
    std::cout << "Earth orbits Sun, Moon orbits Earth!" << std::endl;
}

bool Application::initialize()
{
    if (!initGLFW()) {
        return false;
    }
    
    if (!initGLEW()) {
        return false;
    }
    
    printSystemInfo();
    
    setupScenes();
    
    isRunning = true;
    lastFrameTime = glfwGetTime();
    
    return true;
}

void Application::run()
{
    std::cout << "Application::run() started" << std::endl;

    while (isOpen() && isRunning) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        processInput(deltaTime);

        Scene* currentScene = sceneManager.getCurrentScene();
        if (currentScene) {
            currentScene->update(deltaTime);
        }

        processInput(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (currentScene) {
            currentScene->render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::shutdown()
{
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Application* app = s_instance;
    if (!app) return;

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            app->rightMousePressed = true;
            glfwGetCursorPos(window, &app->lastMouseX, &app->lastMouseY);
        }
        else if (action == GLFW_RELEASE) {
            app->rightMousePressed = false;
        }
    }
}

void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Application* app = s_instance;
    if (!app) return;

    if (!app->rightMousePressed) {
        app->lastMouseX = xpos;
        app->lastMouseY = ypos;
        return;
    }

    double xOffset = xpos - app->lastMouseX;
    double yOffset = app->lastMouseY - ypos;

    app->lastMouseX = xpos;
    app->lastMouseY = ypos;

    float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    Scene* currentScene = app->sceneManager.getCurrentScene();
    if (!currentScene) return;

    Camera* camera = currentScene->getCamera();
    if (!camera) return;

    camera->updateDirection(static_cast<float>(xOffset), static_cast<float>(yOffset));

    //std::cout << "Camera rotated: yaw=" << xOffset << ", pitch=" << yOffset << std::endl;
}

float Application::randomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}