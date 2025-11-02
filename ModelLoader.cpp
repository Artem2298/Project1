#include "ModelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <glm/ext/vector_float2.hpp>

#include "tiny_obj_loader.h"

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

std::vector<float> ModelLoader::loadFromHeader(const std::string& filePath, const std::string& arrayName)
{
    std::string content = readFile(filePath);

    if (content.empty())
    {
        return std::vector<float>();
    }

    return parseFloatArray(content, arrayName);
}

std::vector<float> ModelLoader::loadFromText(const std::string& filePath)
{
    std::cout << "ModelLoader: Loading from text file: " << filePath << std::endl;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file: " << filePath << std::endl;
        return std::vector<float>();
    }

    std::vector<float> vertices;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;

        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::string cleaned;
        for (char c : line) {
            if (c == 'f' || c == 'F') {
                continue;
            }
            else if (c == ',') {
                cleaned += ' ';
            }
            else {
                cleaned += c;
            }
        }

        std::istringstream iss(cleaned);
        float x, y, z, nx, ny, nz;

        if (iss >> x >> y >> z >> nx >> ny >> nz) {
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
        else {
            std::cerr << "WARNING: Invalid format at line " << lineNumber << ": " << line << std::endl;
        }
    }

    file.close();

    std::cout << "ModelLoader: Loaded " << (vertices.size() / 6) << " vertices from text file" << std::endl;

    return vertices;
}

std::vector<float> ModelLoader::loadFromOBJ(const std::string& filePath)
{
    std::cout << "ModelLoader: Loading OBJ file with TinyOBJLoader: " << filePath << std::endl;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::string mtl_basedir = "./";
    size_t last_slash = filePath.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        mtl_basedir = filePath.substr(0, last_slash + 1);
    }

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
        filePath.c_str(), mtl_basedir.c_str());

    if (!warn.empty()) {
        std::cout << "TinyOBJ Warning: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "TinyOBJ Error: " << err << std::endl;
    }

    if (!ret) {
        std::cerr << "ERROR: Failed to load OBJ file: " << filePath << std::endl;
        return std::vector<float>();
    }

    std::vector<float> vertices;

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;

        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = shapes[s].mesh.num_face_vertices[f];

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);

                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
                    tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
                    tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

                    vertices.push_back(nx);
                    vertices.push_back(ny);
                    vertices.push_back(nz);
                }
                else {
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                }
            }

            index_offset += fv;
        }
    }

    std::cout << "ModelLoader: Loaded " << (vertices.size() / 6) << " vertices from OBJ" << std::endl;
    std::cout << "  Shapes: " << shapes.size() << std::endl;
    std::cout << "  Materials: " << materials.size() << std::endl;
    std::cout << "  Total vertices in file: " << attrib.vertices.size() / 3 << std::endl;
    std::cout << "  Total normals in file: " << attrib.normals.size() / 3 << std::endl;

    return vertices;
}

std::string ModelLoader::readFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    file.close();

    return buffer.str();
}

std::vector<float> ModelLoader::parseFloatArray(const std::string& content, const std::string& arrayName)
{
    std::vector<float> result;

    size_t arrayPos = content.find(arrayName);
    if (arrayPos == std::string::npos)
    {
        std::cerr << "ERROR: Array '" << arrayName << "' not found in file\n";
        return result;
    }

    size_t openBrace = content.find('{', arrayPos);
    if (openBrace == std::string::npos)
    {
        std::cerr << "ERROR: Could not find opening brace\n";
        return result;
    }

    size_t closeBrace = content.find('}', openBrace);
    if (closeBrace == std::string::npos)
    {
        std::cerr << "ERROR: Could not find closing brace\n";
        return result;
    }

    std::string arrayContent = content.substr(openBrace + 1, closeBrace - openBrace - 1);

    std::stringstream ss(arrayContent);
    std::string token;

    while (ss >> token)
    {
        if (token.empty() || token == ",")
            continue;

        try
        {
            float value = std::stof(token);
            result.push_back(value);
        }
        catch (const std::exception& e)
        {
            std::cerr << "WARNING: Could not parse '" << token << "' as float\n";
        }
    }

    std::cout << "Loaded array '" << arrayName << "': " << result.size() << " floats\n";
    return result;
}