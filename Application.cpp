#include "Application.h"
#include "Triangle.h"
#include "Square.h"
#include "Plane.h"
#include "TranslateTransform.h"
#include "SceneManager.h"
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

    Triangle* triangle1 = new Triangle();
    scene1->addObject(triangle1);

    Square* square = new Square();
    scene1->addObject(square);

    Triangle* triangle2 = new Triangle();
    triangle2->addStaticTransform(new TranslateTransform(glm::vec3(0.0f, 1.0f, 0.0f)));
    scene1->addObject(triangle2);

    Plane* plane = new Plane();
    scene1->addObject(plane);

    printf("Scene 1 setup complete. Objects in scene: %zu\n", scene1->getObjectCount());

    Scene* scene2 = new Scene();
    sceneManager.addScene(2, scene2);

    glm::mat4 view2 = glm::lookAt(
        glm::vec3(0, 2, 3),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
    glm::mat4 projection2 = glm::perspective(60.0f, aspect, 0.1f, 100.0f);
    scene2->setViewMatrix(view1);
    scene2->setProjectionMatrix(projection1);

    Triangle* triangle3 = new Triangle();
    triangle3->addDynamicTransform(new DynamicRotateTransform(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f));
    scene2->addObject(triangle3);

    Plane* plane2 = new Plane();
    scene2->addObject(plane2);

    printf("Scene 2 setup complete. Objects in scene: %zu\n", scene2->getObjectCount());

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