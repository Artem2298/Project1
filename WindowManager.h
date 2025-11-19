#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Application;

class WindowManager
{
private:
    GLFWwindow* window;
    int width;
    int height;
    const char* title;
    
    Application* app;
    
    bool initGLFW();
    bool initGLEW();
    void printSystemInfo();
    
    static void errorCallback(int error, const char* description);
    static void windowSizeCallback(GLFWwindow* window, int width, int height);
    
    static WindowManager* s_instance;
    
public:
    WindowManager(int width, int height, const char* title, Application* application);
    ~WindowManager();
    
    bool initialize();
    void pollEvents();
    void swapBuffers();
    bool shouldClose() const;
    void setShouldClose(bool value);
    
    GLFWwindow* getWindow() const { return window; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    void updateSize(int w, int h);
};