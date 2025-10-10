#include "Model.h"

Model::Model()
    : VAO(0), VBO(0), vertexCount(0), isLoaded(false)
{
}

Model::~Model()
{
    cleanup();
}

void Model::load(const float* vertices, unsigned int count)
{
    if (!vertices || count == 0)
    {
        std::cerr << "ERROR: Invalid vertices or count = 0\n";
        return;
    }

    if (isLoaded)
        cleanup();

    vertexCount = count;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    isLoaded = true;
    std::cout << "Model loaded successfully: " << count << " vertices\n";
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
    isLoaded = false;
    std::cout << "Model cleaned up\n";
}