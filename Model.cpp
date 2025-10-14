#include "Model.h"

Model::Model()
    : VAO(0), VBO(0), vertexCount(0), stride(3), isLoaded(false)
{
}

Model::~Model()
{
    cleanup();
}

void Model::loadWithStride(const float* vertices, unsigned int vertexCount, GLuint vertexSize)
{
    if (!vertices || vertexCount == 0 || vertexSize == 0)
    {
        std::cerr << "ERROR: Invalid vertices, vertexCount, or vertexSize\n";
        return;
    }

    if (isLoaded)
        cleanup();

    this->vertexCount = vertexCount;
    this->stride = vertexSize;

    GLuint totalFloats = vertexCount * vertexSize;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, totalFloats * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint strideBytes = vertexSize * sizeof(float);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strideBytes, (GLvoid*)0);

    if (vertexSize >= 6)
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strideBytes, (GLvoid*)(3 * sizeof(float)));
    }

    if (vertexSize >= 8)
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, strideBytes, (GLvoid*)(6 * sizeof(float)));
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    isLoaded = true;
    std::cout << "Model loaded with stride " << vertexSize << ": " << vertexCount << " vertices\n";
}

void Model::load(const float* vertices, unsigned int vertexCount)
{
    loadWithStride(vertices, vertexCount, 3);
}

void Model::load(const std::vector<glm::vec3>& vertices)
{
    if (vertices.empty())
    {
        std::cerr << "ERROR: Empty vertices vector\n";
        return;
    }

    std::vector<float> vertexData;
    vertexData.reserve(vertices.size() * 3);

    for (const auto& vertex : vertices)
    {
        vertexData.push_back(vertex.x);
        vertexData.push_back(vertex.y);
        vertexData.push_back(vertex.z);
    }

    load(vertexData.data(), vertices.size());
}

void Model::draw() const
{
    if (!isLoaded)
    {
        std::cerr << "ERROR: Model is not loaded. Call load() first!\n";
        return;
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Model::cleanup()
{
    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    vertexCount = 0;
    stride = 3;
    isLoaded = false;
    std::cout << "Model cleaned up\n";
}