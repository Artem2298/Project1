#include "Model.h"
#include "ShaderProgram.h"

Model::Model()
    : VAO(0), VBO(0), vertexCount(0), stride(3), isLoaded(false)
{
}

Model::~Model()
{
    cleanup();
}

void Model::loadWithStride(const float* vertices, unsigned int count, GLuint vertexSize, ShaderProgram* shader)
{
    if (vertices == nullptr || count == 0) {
        std::cerr << "Model::loadWithStride() - Invalid vertex data!" << std::endl;
        return;
    }

    this->vertexCount = count;
    this->stride = vertexSize;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);

    if (shader != nullptr) {
        GLint positionLoc = shader->getPositionAttribLocation();
        GLint normalLoc = shader->getNormalAttribLocation();
        GLint texCoordLoc = shader->getTexCoordAttribLocation();

        if (positionLoc != -1) {
            glEnableVertexAttribArray(positionLoc);
            glVertexAttribPointer(
                positionLoc,
                3,
                GL_FLOAT,
                GL_FALSE,
                stride * sizeof(float),
                (void*)0
            );
        }
        else {
            std::cout << "   ⚠️  Position attribute not found in shader" << std::endl;
        }

        if (normalLoc != -1 && stride >= 6) {
            glEnableVertexAttribArray(normalLoc);
            glVertexAttribPointer(
                normalLoc,
                3,
                GL_FLOAT,
                GL_FALSE,
                stride * sizeof(float),
                (void*)(3 * sizeof(float))
            );
        }
        else if (normalLoc != -1) {
            std::cout << "   ⚠️  Normal attribute found but stride too small" << std::endl;
        }

        if (texCoordLoc != -1 && stride >= 8) {
            glEnableVertexAttribArray(texCoordLoc);
            glVertexAttribPointer(
                texCoordLoc,
                2,
                GL_FLOAT,
                GL_FALSE,
                stride * sizeof(float),
                (void*)(6 * sizeof(float))
            );
            std::cout << "   ✅ TexCoord attribute configured" << std::endl;
        }
        else if (texCoordLoc != -1) {
            std::cout << "   ℹ️  TexCoord attribute found but stride too small (need 8, have " << stride << ")" << std::endl;
        }

    }
    else {
        std::cout << "⚠️  Model::loadWithStride() - No shader provided, using legacy hardcoded locations" << std::endl;

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);

        if (stride >= 6) {
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
        }

        if (stride >= 8) {
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    isLoaded = true;
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
}