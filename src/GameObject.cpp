#include "GameObject.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <Light.hpp>
#include <Log.hpp>
#include <Material.hpp>
#include <Model.hpp>
#include <Shader.hpp>
#include <Texture.hpp>

#include <imgui/imgui.h>
#include <nlohmann/json.hpp>
#include <stb/stb_image.h>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tinygltf/tiny_gltf.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GameObject::GameObject()
{
}

GameObject::GameObject(glm::vec3 position)
{
    SetPosition(position);
}

GameObject::~GameObject()
{
    _mChildren.clear();
    delete _mModel;
}

void GameObject::Update(const float dt)
{
}

void GameObject::Render()
{
    if (_mShader != nullptr && _mModel != nullptr)
    {
        _mModel->Render(_mShader, GetWorldTransform());
    }
}

void GameObject::SetShader(Shader* shader)
{
    _mShader = shader;
}

bool GameObject::Load(std::string filename)
{
    std::string ext = Utils::GetExtension(filename);
    bool binary = (ext == "glb");

    std::string fullPath;
    const auto& paths = Utils::GetResourcePaths();

    Assimp::Importer importer;
    const aiScene* scene;
    std::string err, warn;

    bool loaded = false;
    for (auto& p : paths)
    {
        fullPath = p + filename;

        LogVerbose("Checking %s\n", fullPath);
        scene = importer.ReadFile(fullPath, aiProcessPreset_TargetRealtime_Quality);

        if (scene)
        {
            LogLoad("Loading: %s\n", fullPath);
            loaded = true;
        }

        if (loaded) break;
    }

    if (!loaded)
    {
        LogError("Error: Loading %s in Assimp.\n", fullPath);
    }

    _mScene = scene;

    if (_mScene->HasTextures())
    {
        bool loadedTextures = processTextures();

        if (!loadedTextures)
        {
            LogError("Error: Could not load textures in: %s\n", fullPath);
            return false;
        }
    }

    if (_mScene->HasMaterials())
    {
        bool loadedMaterials = processMaterials();

        if (!loadedMaterials)
        {
            LogError("Error: Could not load materials in: %s\n", fullPath);
            return false;
        }
    }

    //bool loadedMaterials = processMaterials();

    processNode(scene->mRootNode);

    glBindVertexArray(0);

    for (auto vbo : _mVBOS)
    {
        glDeleteBuffers(1, &vbo);
    }

    _mTextures.clear();
    _mMaterials.clear();
    
    return true;
}

bool GameObject::processTextures()
{
    for (unsigned int i = 0; i < _mScene->mNumTextures; i++)
    {
        aiTexture* texture = _mScene->mTextures[i];
        LogVerbose("Loading Texture %s: %f, %f", texture->mFilename.C_Str(), texture->mHeight, texture->mWidth);
        _mTextures.push_back(std::make_shared<Texture>(texture->mFilename.C_Str()));
    }

    return true;
}

bool GameObject::processMaterials()
{
    if (_mScene->HasMaterials())
    {
        for (int i = 0; i < _mScene->mNumMaterials; i++)
        {
            auto material = _mScene->mMaterials[i];
            
        }
    }
    else
    {
        return false;
    }

    return true;
}

std::unique_ptr<GameObject> GameObject::processNode(aiNode* node)
{
    GameObject* gobj = nullptr;

    //if (lightcrap/cameracrap)
    //else
    gobj = new GameObject();

    if (node->mNumMeshes >= 0)
    {
        std::vector<Mesh*> meshes;

        // Process our game objects meshes
        for (int i = 0; i < node->mNumMeshes; i++)
        {
            auto& mesh = _mScene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh));
        }

        // Process the childrens meshes
        for (int i = 0; i < node->mNumMeshes; i++)
        {
            processNode(node->mChildren[i]);
        }
    }
    return std::unique_ptr<GameObject>();
}

Mesh* GameObject::processMesh(aiMesh* mesh)
{
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    bool hasNormals = mesh->HasNormals();
    bool hasTexCoords = mesh->HasTextureCoords(0);
    bool hasTangentsAndBitangents = mesh->HasTangentsAndBitangents();

    // Get every indice
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            const auto& vert = mesh->mVertices[face.mIndices[j]];
            vertices.push_back({ vert.x, vert.y, vert.z });

            if (hasNormals) {
                const auto& norm = mesh->mNormals[face.mIndices[j]];
                normals.push_back({ norm.x, norm.y, norm.z });
            }

            if (hasTexCoords) {
                const auto& txcd = mesh->mTextureCoords[0][face.mIndices[j]];
                texCoords.push_back({ txcd.x, txcd.y });
            }

            if (hasTangentsAndBitangents) {
                const auto& tangent = mesh->mTextureCoords[0][face.mIndices[j]];
                tangents.push_back({ tangent.x, tangent.y, tangent.z });

                const auto& bitangent = mesh->mTextureCoords[0][face.mIndices[j]];
                bitangents.push_back({ bitangent.x, bitangent.y, bitangent.z });
            }
        }
    }

    GLuint vao, vbo;

    // VAOs
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VBOs
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, vertices.data(), GL_STATIC_DRAW);

    // Attributes
    glVertexAttribPointer(AttributeID::POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AttributeID::POSITION);
    _mVBOS.push_back(vbo);

    // Normals
    if (hasNormals)
    {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size() * 3, normals.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(AttributeID::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AttributeID::NORMAL);
        _mVBOS.push_back(vbo);
    }

    // TexCoords
    if (hasTexCoords)
    {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size() * 2, texCoords.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(AttributeID::TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AttributeID::TEXCOORD);
        _mVBOS.push_back(vbo);
    }

    if (hasTangentsAndBitangents)
    {
        // Tangents
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tangents.size() * 3, tangents.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(AttributeID::TANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AttributeID::TANGENT);
        _mVBOS.push_back(vbo);

        // Bitangents
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bitangents.size() * 3, bitangents.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(AttributeID::BITANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AttributeID::BITANGENT);
        _mVBOS.push_back(vbo);
    }

    // Create a new mesh
    meshes.push_back(new Mesh(vao,
        GL_TRIANGLES,
        GL_UNSIGNED_INT,
        NULL,
        _mMaterials[(primitive.material < 0 ? 0 : primitive.material)]));
    
    // create the material

    // do material things

    //Mesh* newMesh = new Mesh(vertices, normals, texCoords, tangents, bitangents);
    //Material* newMat = nullptr;

    //// Materials
    //if (mesh->mMaterialIndex >= 0)
    //{
    //    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    //    // Ambient
    //    std::string ambientTex = GetMaterialTextureName(material, aiTextureType_AMBIENT, dirname);

    //    // Diffuse
    //    std::string diffuseTex = GetMaterialTextureName(material, aiTextureType_DIFFUSE, dirname);

    //    // Specular
    //    std::string specularTex = GetMaterialTextureName(material, aiTextureType_SPECULAR, dirname);

    //    // Normal
    //    std::string normalTex = GetMaterialTextureName(material, aiTextureType_NORMALS, dirname);

    //    aiColor4D aiAmb;
    //    aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &aiAmb);
    //    float amb[3] = { aiAmb.r, aiAmb.g, aiAmb.b };

    //    aiColor4D aiDiff;
    //    aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &aiDiff);
    //    float diff[3] = { aiDiff.r, aiDiff.g, aiDiff.b };

    //    aiColor4D aiSpec;
    //    aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &aiSpec);
    //    float spec[3] = { aiSpec.r, aiSpec.g, aiSpec.b };

    //    aiColor4D aiShininess;
    //    aiGetMaterialColor(material, AI_MATKEY_SHININESS, &aiShininess);
    //    glm::vec3 shininess(aiShininess.r, aiShininess.g, aiShininess.b);

    //    // Notes:
    //    // Remove dissolve from material it's pointless
    //    // Add all the other materials, Albedo, Metallic, roughness, AO?, etc


    //    newMat = new Material(amb, diff, spec, aiShininess.r, ambientTex, diffuseTex, specularTex, normalTex);
    //    newMesh->SetMaterial(newMat);
    //}

    //return newMesh;

    //return Mesh(vertices, indices, textures);

    return nullptr;
}

//std::string GameObject::GetMaterialTextureName(aiMaterial* material, aiTextureType type, std::string dirname)
//{
//    std::string texName;
//    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
//    {
//        aiString str;
//        material->GetTexture(type, i, &str);
//        texName = dirname + str.C_Str();
//    }
//    return texName;
//}


