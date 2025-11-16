#include "Texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
    : textureID(0)
    , width(0)
    , height(0)
    , channels(0)
    , isLoaded(false)
{
}

Texture::~Texture()
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

bool Texture::loadFromFile(const std::string& filepath)
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    std::cout << "Loading texture: " << filepath << std::endl;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 4);

    if (!data) {
        std::cerr << "ERROR: Failed to load texture: " << filepath << std::endl;
        std::cerr << "Reason: " << stbi_failure_reason() << std::endl;
        return false;
    }

    std::cout << "  Size: " << width << "x" << height << std::endl;
    std::cout << "  Channels: " << channels << " (loaded as RGBA)" << std::endl;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(
        GL_TEXTURE_2D,     
        0,               
        GL_RGBA,
        width,
        height, 
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    isLoaded = true;
    std::cout << "? Texture loaded successfully (ID: " << textureID << ")" << std::endl;

    return true;
}

void Texture::bind(GLuint textureUnit) const
{
    if (!isLoaded) {
        std::cerr << "??  Warning: Trying to bind unloaded texture!" << std::endl;
        return;
    }

    glActiveTexture(GL_TEXTURE0 + textureUnit);

    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}