

#include "Models.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>

void ModelClass::loadObj()
{
    // Loading .obj file
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &materials,
        &warning,
        &error,
        this->objPath.c_str());

    // Loading tangents and bitangents
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3)
    {
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[vData1.vertex_index * 3 + 1],
            attributes.vertices[vData1.vertex_index * 3 + 2]);

        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[vData2.vertex_index * 3 + 1],
            attributes.vertices[vData2.vertex_index * 3 + 2]);

        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[vData3.vertex_index * 3 + 1],
            attributes.vertices[vData3.vertex_index * 3 + 2]);

        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[vData1.texcoord_index * 2],
            attributes.texcoords[vData1.texcoord_index * 2 + 1]);

        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[vData2.texcoord_index * 2],
            attributes.texcoords[vData2.texcoord_index * 2 + 1]);

        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[vData3.texcoord_index * 2],
            attributes.texcoords[vData3.texcoord_index * 2 + 1]);

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

        glm::vec3 tangent = r * (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y);
        glm::vec3 bitangent = r * (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x);

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }

    // Loading vertex data
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];
        int vertexIndex = vData.vertex_index * 3;
        int normalIndex = vData.normal_index * 3;
        int uvIndex = vData.texcoord_index * 2;

        // ---------------------------------------------------
        // POSITION
        this->vertexData.push_back(
            attributes.vertices[vertexIndex]);

        this->vertexData.push_back(
            attributes.vertices[vertexIndex + 1]);

        this->vertexData.push_back(
            attributes.vertices[vertexIndex + 2]);

        // ---------------------------------------------------
        // NORMALS
        this->vertexData.push_back(
            attributes.normals[normalIndex]);

        this->vertexData.push_back(
            attributes.normals[normalIndex + 1]);

        this->vertexData.push_back(
            attributes.normals[normalIndex + 2]);

        // ---------------------------------------------------
        // TEXTURE COORDINATES
        this->vertexData.push_back(
            attributes.texcoords[uvIndex]);

        this->vertexData.push_back(
            attributes.texcoords[uvIndex + 1]);

        // ---------------------------------------------------
        // TANGENTS

        this->vertexData.push_back(
            tangents[i].x);
        this->vertexData.push_back(
            tangents[i].y);
        this->vertexData.push_back(
            tangents[i].z);

        // ---------------------------------------------------
        // BITANGENTS

        this->vertexData.push_back(
            bitangents[i].x);
        this->vertexData.push_back(
            bitangents[i].y);
        this->vertexData.push_back(
            bitangents[i].z);
    }
}
void ModelClass::attachTexture(std::string texPath, GLint format)
{
    // Flip image vertically on load
    stbi_set_flip_vertically_on_load(true);

    // Initialize variables for loading the texture
    int img_width, img_height, colorChannels;

    unsigned char *tex_bytes = stbi_load(
        texPath.c_str(),
        &img_width, &img_height, &colorChannels,
        0);

    // Initialize texture variable
    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Attach loaded image to texture variable
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        img_width,
        img_height,
        0,
        (colorChannels == 3) ? GL_RGB : GL_RGBA,
        GL_UNSIGNED_BYTE,
        tex_bytes);
    // Generate Mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free up loaded bytes
    stbi_image_free(tex_bytes);

    this->textures.push_back(tex);
}

void ModelClass::attachNormalTexture(std::string texPath, GLint format)
{
    this->withNormals = true;
    attachTexture(texPath, format);
}

void ModelClass::createVAO_VBO()
{
    GLintptr normalPtr = 3 * sizeof(GL_FLOAT);
    GLintptr uvPtr = 6 * sizeof(GL_FLOAT);
    GLintptr tangentPtr = 8 * sizeof(GL_FLOAT);
    GLintptr bitangentPtr = 11 * sizeof(GL_FLOAT);

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * this->vertexData.size(),
        this->vertexData.data(),
        GL_STATIC_DRAW);

    // Vertices
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void *)0);

    // Normals
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void *)normalPtr);

    // Texture coordinates
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void *)uvPtr);

    // Tangents
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void *)tangentPtr);

    // Bitangents
    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void *)bitangentPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}