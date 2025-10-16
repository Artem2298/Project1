#include "Application.h"
#include "Triangle.h"
#include "Square.h"
#include "Plane.h"
#include "TranslateTransform.h"
#include "DynamicRotateTransform.h"
#include "SceneManager.h"
#include "ModelLoader.h"
#include "ScaleTransform.h"
#include "RotateTransform.h"
#include "ModelCache.h"
#include <iostream>
#include <glm/vec3.hpp>
#include <random>
#include <ctime>

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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS && s_instance)
    {
        if (key == GLFW_KEY_1)
            s_instance->getSceneManager().switchScene(1);
        else if (key == GLFW_KEY_2)
            s_instance->getSceneManager().switchScene(2);
        else if (key == GLFW_KEY_3)
            s_instance->getSceneManager().switchScene(3);
    }
}

void Application::processInput(float deltaTime)
{
    if (!window) return;
    Scene* scene = getSceneManager().getCurrentScene();
    Camera* camera = scene ? scene->getCamera() : nullptr;
    if (!camera) return;

    glm::vec3 direction = glm::normalize(camera->getDirection());
    glm::vec3 up = glm::normalize(camera->getUp());
    float dot = glm::dot(direction, up);
    if (std::abs(dot) > 0.95f) {
        return;
    }

    glm::vec3 right = glm::cross(direction, up);
    float rightLength = glm::length(right);
    if (rightLength < 0.001f) {
        return;
    }
    right = right / rightLength;

    glm::vec3 eye = camera->getEye();
    float moveSpeed = 6.0f;
    glm::vec3 moveDirection(0.0f);
    bool moved = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        moveDirection += direction;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        moveDirection -= direction;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        moveDirection -= right;
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        moveDirection += right;
        moved = true;
    }

    if (moved)
    {
        float moveLength = glm::length(moveDirection);
        if (moveLength > 0.001f)
        {
            moveDirection = moveDirection / moveLength;
        }
        else
        {
            return;
        }

        glm::vec3 newPosition = eye + moveDirection * moveSpeed * deltaTime;

        if (std::isnan(newPosition.x) || std::isnan(newPosition.y) || std::isnan(newPosition.z) ||
            std::isinf(newPosition.x) || std::isinf(newPosition.y) || std::isinf(newPosition.z))
        {
            return;
        }

        //std::cout << "\n[User Action] Camera position changed by keyboard\n";
        camera->setPosition(newPosition);

        static float timeSinceLastLog = 0.0f;
        timeSinceLastLog += deltaTime;
        if (timeSinceLastLog > 0.5f)
        {
            std::cout << "Camera now at: (" << newPosition.x << ", "
                << newPosition.y << ", " << newPosition.z << ")\n";
            timeSinceLastLog = 0.0f;
        }
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

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

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

bool Application::loadShaders(const char* vertexPath, const char* fragmentPath)
{
    shaderProgram = std::make_unique<ShaderProgram>();

    if (!shaderProgram->loadShader(vertexPath, fragmentPath))
    {
        fprintf(stderr, "ERROR: Failed to load shaders\n");
        return false;
    }

    std::cout << "Shaders loaded successfully\n";
    return true;
}

void Application::setupScene() 
{
    std::cout << "\n========================================\n";
    std::cout << "Setting up scenes with model caching...\n";
    std::cout << "========================================\n\n";

    Scene* scene1 = new Scene();
    sceneManager.addScene(1, scene1);

    Camera* camera1 = new Camera(
        glm::vec3(0.0f, 5.0f, 15.0f),
        glm::vec3(0.0f, -0.3f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        (float)windowWidth / windowHeight,
        0.5f,
        100.0f
    );

    scene1->setCamera(camera1);

    DrawableObject* plane = new DrawableObject(false);
    if (plane->loadModel("models/plain.h", "plain")) {
        plane->addStaticTransform(new ScaleTransform(glm::vec3(40.0f, 1.0f, 40.0f)));
        scene1->addObject(plane);
    }

    std::mt19937 rng(42);
    std::uniform_real_distribution<float> posX(-10.0f, 10.0f);
    std::uniform_real_distribution<float> posZ(-20.0f, 20.0f);
    std::uniform_real_distribution<float> treeScale(0.75f, 1.5f);
    std::uniform_real_distribution<float> bushScale(0.4f, 1.2f);
    std::uniform_real_distribution<float> rotation(0.0f, 360.0f);

    auto treeStartTime = glfwGetTime();

    for (int i = 0; i < 50; i++) {
        DrawableObject* tree = new DrawableObject(false);
        if (tree->loadModel("models/tree.h", "tree")) {
            float x = posX(rng);
            float z = posZ(rng);
            float s = treeScale(rng);
            float rot = rotation(rng);

            tree->addStaticTransform(new ScaleTransform(glm::vec3(s, s, s)));
            tree->addStaticTransform(new RotateTransform(glm::vec3(0, 1, 0), rot));
            tree->addStaticTransform(new TranslateTransform(glm::vec3(x, 0.0f, z)));

            scene1->addObject(tree);
        }
    }

    auto treeEndTime = glfwGetTime();

    auto bushStartTime = glfwGetTime();

    for (int i = 0; i < 50; i++) {
        DrawableObject* bush = new DrawableObject(false);
        if (bush->loadModel("models/bushes.h", "bushes")) {
            float x = posX(rng);
            float z = posZ(rng);
            float s = bushScale(rng);
            float rot = rotation(rng);

            bush->addStaticTransform(new ScaleTransform(glm::vec3(s, s, s)));
            bush->addStaticTransform(new RotateTransform(glm::vec3(0, 1, 0), rot));
            bush->addStaticTransform(new TranslateTransform(glm::vec3(x, 0.0f, z)));

            scene1->addObject(bush);
        }
    }

    auto bushEndTime = glfwGetTime();

    printf("Scene 1 setup complete. Objects in scene: %zu\n\n", scene1->getObjectCount());


    Scene* scene2 = new Scene();
    sceneManager.addScene(2, scene2);

    Camera* camera2 = new Camera(
        glm::vec3(0.0f, 0.0f, -7.0f),
        glm::vec3(0.0f, 0.0f, 7.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        (float)windowWidth / windowHeight,
        0.1f,
        100.0f
    );
    scene2->setCamera(camera2);

    DrawableObject* sphere1 = new DrawableObject(true);
    if (sphere1->loadModel("models/sphere.h", "sphere")) {
        sphere1->addStaticTransform(new TranslateTransform(glm::vec3(3, 0, 0)));
        scene2->addObject(sphere1);
    }

    DrawableObject* sphere2 = new DrawableObject(true);
    if (sphere2->loadModel("models/sphere.h", "sphere")) {
        sphere2->addStaticTransform(new TranslateTransform(glm::vec3(-3, 0, 0)));
        scene2->addObject(sphere2);
    }

    DrawableObject* sphere3 = new DrawableObject(true);
    if (sphere3->loadModel("models/sphere.h", "sphere")) {
        sphere3->addStaticTransform(new TranslateTransform(glm::vec3(0, 3, 0)));
        scene2->addObject(sphere3);
    }

    DrawableObject* sphere4 = new DrawableObject(true);
    if (sphere4->loadModel("models/sphere.h", "sphere")) {
        sphere4->addStaticTransform(new TranslateTransform(glm::vec3(0, -3, 0)));
        scene2->addObject(sphere4);
    }

    printf("Scene 2 setup complete. Objects in scene: %zu\n\n", scene2->getObjectCount());

    Scene* scene3 = new Scene();
    sceneManager.addScene(3, scene3);

    Camera* camera3 = new Camera(
        glm::vec3(5.0f, 3.0f, 5.0f),
        glm::vec3(-5.0f, -3.0f, -5.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        (float)windowWidth / windowHeight,
        0.1f,
        100.0f
    );

    scene3->setCamera(camera3);

    for (int i = 0; i < 10; i++) {
        DrawableObject* triangle = new DrawableObject(true);
        if (triangle->loadModel("models/triangle.h", "triangle")) {
            float angle = (360.0f / 10.0f) * i;
            float radius = 2.0f;
            float x = radius * glm::cos(glm::radians(angle));
            float z = radius * glm::sin(glm::radians(angle));
            float y = glm::sin(glm::radians(angle * 2.0f)) * 0.5f;

            triangle->addStaticTransform(new TranslateTransform(glm::vec3(x, y, z)));
            triangle->addDynamicTransform(new DynamicRotateTransform(glm::vec3(1, 1, 0), 30.0f + i * 5.0f));

            scene3->addObject(triangle);
        }
    }

    printf("Scene 3 setup complete. Objects in scene: %zu\n\n", scene3->getObjectCount());

    sceneManager.switchScene(1);
}

bool Application::initialize()
{
    std::cout << "Initializing Application...\n";

    if (!initGLFW())
        return false;

    if (!initGLEW())
        return false;

    printSystemInfo();

    if (!loadShaders("vertex.glsl", "fragment.glsl"))
        return false;

    setupScene();

    isRunning = true;
    lastFrameTime = glfwGetTime();

    lastMouseX = static_cast<double>(windowWidth) / 2.0;
    lastMouseY = static_cast<double>(windowHeight) / 2.0;

    std::cout << "Application initialized successfully\n";
    return true;
}

void Application::run()
{
    if (!isRunning)
    {
        std::cerr << "ERROR: Application not initialized. Call initialize() first\n";
        return;
    }

    while (isOpen())
    {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        processInput(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Scene* currentScene = sceneManager.getCurrentScene();
        if (currentScene) {
            currentScene->update(deltaTime);
            currentScene->render(*shaderProgram);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::shutdown()
{
    std::cout << "Shutting down Application...\n";

    shaderProgram.reset();

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
    isRunning = false;

    std::cout << "Application shutdown complete\n";
}

void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (s_instance && button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        s_instance->rightMousePressed = (action == GLFW_PRESS);
    }
}

void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (s_instance && s_instance->rightMousePressed)
    {
        float deltaYaw = static_cast<float>(xpos - s_instance->lastMouseX) * 0.001f;
        float deltaPitch = static_cast<float>(ypos - s_instance->lastMouseY) * 0.001f;

        Scene* scene = s_instance->getSceneManager().getCurrentScene();
        Camera* camera = scene ? scene->getCamera() : nullptr;
        if (camera)
        {
            camera->updateDirection(-deltaYaw, deltaPitch);
        }
    }

    s_instance->lastMouseX = xpos;
    s_instance->lastMouseY = ypos;
}

float randomFloat(float min, float max)
{
    static std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}
