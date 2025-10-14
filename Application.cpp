#include "Application.h"
#include "Triangle.h"
#include "Square.h"
#include "Plane.h"
#include "TranslateTransform.h"
#include "DynamicRotateTransform.h"
#include "SceneManager.h"
#include "ModelLoader.h"
#include <iostream>
#include <glm/vec3.hpp>

Application* Application::s_instance = nullptr;

Application::Application(int width, int height, const char* title)
    : window(nullptr),
    windowWidth(width),
    windowHeight(height),
    windowTitle(title),
    isRunning(false),
    lastFrameTime(0.0)
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
    Scene* scene1 = new Scene();
    sceneManager.addScene(1, scene1);

    glm::mat4 view1 = glm::lookAt(
        glm::vec3(0, 2, 3),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );

    float aspect = windowWidth / (float)windowHeight;
    glm::mat4 projection1 = glm::perspective(45.0f, aspect, 0.1f, 100.0f);

    scene1->setViewMatrix(view1);
    scene1->setProjectionMatrix(projection1);

    DrawableObject* triangle1 = new DrawableObject(true);
    if (triangle1->loadModel("models/triangle.h", "triangle")) {
        triangle1->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0, 1, 0), 50.0f));
        triangle1->addStaticTransform(new TranslateTransform(glm::vec3(-0.5f, 0.5f, 0.0f)));
        scene1->addObject(triangle1);
    }

    DrawableObject* square = new DrawableObject(false);
    if (square->loadModel("models/square.h", "square")) {
        square->addStaticTransform(new TranslateTransform(glm::vec3(0.5f, 0.5f, 0.0f)));
        scene1->addObject(square);
    }

    DrawableObject* triangle2 = new DrawableObject(true);
    if (triangle2->loadModel("models/triangle.h", "triangle")) {
        triangle2->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0, 1, 0), 30.0f));
        triangle2->addStaticTransform(new TranslateTransform(glm::vec3(0.0f, 1.0f, 0.0f)));
        scene1->addObject(triangle2);
    }

    DrawableObject* plane = new DrawableObject(false);
    if (plane->loadModel("models/plane.h", "plane")) {
        scene1->addObject(plane);
    }

    printf("Scene 1 setup complete. Objects in scene: %zu\n", scene1->getObjectCount());

    Scene* scene2 = new Scene();
    sceneManager.addScene(2, scene2);

    glm::mat4 view2 = glm::lookAt(
        glm::vec3(0, 1, 2),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );

    glm::mat4 projection2 = glm::perspective(60.0f, aspect, 0.1f, 100.0f);

    scene2->setViewMatrix(view2);
    scene2->setProjectionMatrix(projection2);

    DrawableObject* triangle3 = new DrawableObject(true);
    if (triangle3->loadModel("models/triangle.h", "triangle")) {
        triangle3->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0, 1, 0), 30.0f));
        scene2->addObject(triangle3);
    }

    DrawableObject* plane2 = new DrawableObject(false);
    if (plane2->loadModel("models/plane.h", "plane")) {
        scene2->addObject(plane2);
    }

    printf("Scene 2 setup complete. Objects in scene: %zu\n", scene2->getObjectCount());

    Scene* scene3 = new Scene();
    sceneManager.addScene(3, scene3);

    glm::mat4 view3 = glm::lookAt(
        glm::vec3(5, 3, 5),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );

    glm::mat4 projection3 = glm::perspective(45.0f, aspect, 0.1f, 100.0f);

    scene3->setViewMatrix(view3);
    scene3->setProjectionMatrix(projection3);

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

    printf("Scene 3 setup complete. Objects in scene: %zu\n", scene3->getObjectCount());

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

    std::cout << "Application running. Press ESC to exit.\n";

    while (isOpen())
    {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

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