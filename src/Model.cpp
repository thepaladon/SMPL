#include "Model.h"

#include "Random.h"

#ifdef RASPBERRY
#include "external/stb_image/stb_image.h"
#elif WIN32
#include "stb_image/stb_image.h"
#endif

unsigned int TextureFromFile(const char* path, const std::string& directory);

Model::Model(Mesh* mesh)
{
    meshes.push_back(mesh);
    usedIn++;
}

Model::~Model()
{
    clearTextures();
    for (auto mesh : meshes)
    {
        if (mesh != nullptr) {
            delete mesh;
            mesh = nullptr;
        }
    }
    
    if (meshes.size() > 0) { meshes.clear(); }
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("ERROR::ASSIMP::%s in %s \n", importer.GetErrorString(), path.c_str());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

	int vertxCounter = 0;
    int indxCounter = 0;

    processNode(scene->mRootNode, scene, vertxCounter, indxCounter);

#ifdef _DEBUG
    // output the data, but wrap in a debug when not used
    printf("Attrib data for %s is : \n", path.c_str());
    printf("# Number of Meshes: %i \n", scene->mNumMeshes);
    printf("# Vertices: %i \n", vertxCounter);
    printf("# Indices: %i \n \n", indxCounter);
#endif
}

void Model::addTexture(Texture tex)
{
    for(auto mesh : meshes)
    {
        mesh->addTexture(tex);
    }
}

void Model::clearTextures()
{
    for (auto mesh : meshes)
    { mesh->m_textures.clear(); }
}

void Model::processNode(aiNode* node, const aiScene* scene, int& counterVertex, int& counterIndex)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    	meshes.push_back(processMesh(mesh, scene, counterVertex, counterIndex));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, counterVertex, counterIndex);
    }

}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene, int& counterVertex, int& counterIndex)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions...
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

		//normals
        vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

        //texture coordinates
        if (mesh->mTextureCoords[0] != nullptr) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;

	        vector.x = mesh->mTangents[i].x;
	        vector.y = mesh->mTangents[i].y;
	        vector.z = mesh->mTangents[i].z;
	        vertex.Tangent = vector;


	        vector.x = mesh->mBitangents[i].x;
	        vector.y = mesh->mBitangents[i].y;
	        vector.z = mesh->mBitangents[i].z;
	        vertex.BiTangent = vector;

        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            vertex.Tangent = glm::vec3(0.f, 0.f, 0.f);
            vertex.BiTangent = glm::vec3(0.f, 0.f, 0.f);
        }
        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        counterIndex += face.mNumIndices;
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        if (mesh->mMaterialIndex >= 0)
        {
            
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
            // Same applies to other texture as the following list summarizes:
            // diffuse: texture_diffuseN
            // specular: texture_specularN
            // normal: texture_normalN

            // 1. diffuse maps
           std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, NORMAL);
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        	// 4. height maps
            //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        }
    }

    counterVertex += mesh->mNumVertices;

    //CREATES basic 1x1 textures for when they're missing so it has color
    if(textures.empty())
    {
        bool skipDiff = false;
        bool skipSpec = false;
        bool skipNorm = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) //passing textures that have already been loaded
        {
            if (std::strcmp(textures_loaded[j].getFilepath().data(), "Manual: texture_diffuse") == 0)
            {
                textures.push_back(textures_loaded[j]);
                skipDiff = true;
                break;
            }

            if (std::strcmp(textures_loaded[j].getFilepath().data(), "Manual: texture_normal") == 0)
            {
                textures.push_back(textures_loaded[j]);
                skipNorm = true;
                break;
            }

            if (std::strcmp(textures_loaded[j].getFilepath().data(), "Manual: texture_specular") == 0)
            {
                textures.push_back(textures_loaded[j]);
                skipSpec = true;
                break;
            }
        }

        if (!skipDiff) {
            printf("Diffuse Texture not found for %s, creating manually \n", mesh->mName.C_Str());
            Texture diffuse_texture(DIFFUSE, rand() / RAND_MAX* 255, rand() / RAND_MAX * 255, rand() / RAND_MAX * 255, 255);
            //Texture diffuse_texture(DIFFUSE, NEAREST, 1000, 1000, IRand(255), IRand(255), IRand(255) );
            textures.push_back(diffuse_texture);
            textures_loaded.push_back(diffuse_texture); // add to loaded textures
        }

        if (!skipNorm) {
            printf("Normal map not found for %s, creating manually \n", mesh->mName.C_Str());
            Texture normal_texture(NORMAL, 128, 128, 255, 255);
            textures.push_back(normal_texture);
            textures_loaded.push_back(normal_texture); // add to loaded textures
        }

        if (!skipSpec) {
            printf("Specular map not found for %s, creating manually \n", mesh->mName.C_Str());
            Texture specular_texture(SPECULAR, 255, 255, 255, 255);
            textures.push_back(specular_texture);
            textures_loaded.push_back(specular_texture); // add to loaded textures
        }
    }

    return new Mesh(vertices, indices, textures);

}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) //passing textures that have already been loaded
        {
            if (std::strcmp(textures_loaded[j].getFilepath().data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture(directory + '/' + str.C_Str());
            texture.setType(typeName);
            texture.setFilepath(str.C_Str());
            printf("Loaded: %s \n", str.C_Str());
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;

}


