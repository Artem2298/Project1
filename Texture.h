#pragma once
#include <GL/glew.h>
#include <string>

class Texture
{
private:
    GLuint textureID;
    int width;
    int height;
    int channels;
    bool isLoaded;

public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string& filepath);

    void bind(GLuint textureUnit = 0) const;

    void unbind() const;

    GLuint getID() const { return textureID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
    bool isTextureLoaded() const { return isLoaded; }

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
};