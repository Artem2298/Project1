#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/vec3.hpp>
#include <iostream>

class ShaderProgram;

class Model
{
private:
    GLuint VAO;
    GLuint VBO;
    GLuint vertexCount;
    GLuint stride;
    bool isLoaded;

public:
    Model();
    ~Model();

    //void load(const float* vertices, unsigned int vertexCount);

    //void load(const std::vector<glm::vec3>& vertices);

    void loadWithStride(const float* vertices, unsigned int vertexCount, GLuint vertexSize, ShaderProgram* shader = nullptr);

    void draw() const;

    GLuint getVAO() const { return VAO; }
    GLuint getVBO() const { return VBO; }
    unsigned int getVertexCount() const { return vertexCount; }
    bool isModelLoaded() const { return isLoaded; }

    void cleanup();
};