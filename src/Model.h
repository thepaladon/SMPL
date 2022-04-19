#pragma once
#include <vector>

#include "Shader.h"
#include "Mesh.h"

#ifdef RASPBERRY
//assimp headers
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#elif WIN32
//assimp header
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif

class Model
{
public:
    Model(const std::string path)
    {
        loadModel(path);
        usedIn++;
    }
    Model(Mesh* mesh);
    ~Model();

    size_t getSizeOfMeshes() { return meshes.size(); }
    Mesh* getMeshes(int i)  { return meshes[i]; };
    void addTexture(Texture tex);
    void clearTextures();

private:
    // model data
    std::vector<Mesh*> meshes;
    std::string directory;
    int usedIn = 0;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene, int& counterVertex, int& counterIndex);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene, int& counterVertex, int& counterIndex);
    std::vector<Texture> textures_loaded;
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
};

