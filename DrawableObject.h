#pragma once
#include "Model.h"
#include "Transformation.h"
#include "ShaderProgram.h"
#include "ModelLoader.h"
#include "Texture.h"
#include <glm/vec3.hpp>

class DrawableObject
{
protected:
    Model model;
    Transformation transform;
    ModelLoader modelLoader;
    ShaderProgram* shader;
    int objectID;

    glm::vec3 objectColor;
    float shininess;

    Texture* texture;


public:
    DrawableObject(bool isDynamic = false);
    virtual ~DrawableObject();

    virtual void update(float deltaTime);
    virtual void draw();

    bool loadModel(const std::string& filePath, const std::string& arrayName);

    bool loadModelFromText(const std::string& filePath);
    bool loadModelFromOBJ(const std::string& filePath);

    void addStaticTransform(ITransformComponent* component);
    void addDynamicTransform(ITransformComponent* component);

    void setShader(ShaderProgram* shaderProgram);
    ShaderProgram* getShader() const { return shader; }

    void setObjectColor(const glm::vec3& color) { objectColor = color; }
    glm::vec3 getObjectColor() const { return objectColor; }

    void setShininess(float shine) { shininess = shine; }
    float getShininess() const { return shininess; }

    Transformation& getTransformation() { return transform; }
    const Transformation& getTransformation() const { return transform; }

    Model& getModel() { return model; }
    const Model& getModel() const { return model; }

    glm::mat4 getModelMatrix() const { return transform.getMatrix(); }

    void setTexture(Texture* tex) { texture = tex; }
    Texture* getTexture() const { return texture; }

    void setID(int id) { objectID = id; }
    int getID() const { return objectID; }
};