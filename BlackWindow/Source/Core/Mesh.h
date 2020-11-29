#pragma once
#include <string>
#include <vector>
#include <assimp/scene.h>
#include "Texture.h"
#include "Vertex.h"

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int> &indices,const std::vector<Texture> &textures);

public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    void Draw(const unsigned int shader);
    void ChangeTextures(const std::vector<Texture>& textures);

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();

};
