#include "GameObject.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <Light.hpp>
#include <Log.hpp>
#include <Mesh.hpp>
#include <Utils.hpp>

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

bool GameObject::Load(std::string filename)
{
    std::string ext = Utils::GetExtension(filename);
    bool binary = (ext == "glb");

    std::string fullPath;
    const auto& paths = Utils::GetResourcePaths();

    Assimp::Importer importer;
    const aiScene * scene;
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
        return false;
    }

    std::string dir = Utils::GetDirname(fullPath);

    processNode(scene, dir, scene->mRootNode);
    
    return true;
}

std::unique_ptr<GameObject> GameObject::processNode(const aiScene * scene, std::string dir, aiNode* node)
{
    auto gobj = std::make_unique<GameObject>();

    //if (lightcrap/cameracrap)
    //else

    if (node->mNumMeshes >= 0)
    {
        std::vector<std::unique_ptr<Mesh>> meshes;

        // Process our game objects meshes
        for (int i = 0; i < node->mNumMeshes; i++)
        {
            auto& mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(scene, dir, mesh));
        }

        if (!meshes.empty())
        {
            gobj->SetModel(std::make_unique<Model>(std::move(meshes)));
        }

        if (node->mNumChildren > 0)
        {
            // Process the childrens meshes
            for (int i = 0; i < node->mNumMeshes; i++)
            {
                gobj->AddChild(processNode(scene, dir, node->mChildren[i]));
            }
        }
    }

    return gobj;
}

std::unique_ptr<Mesh> GameObject::processMesh(const aiScene * scene, std::string dir, aiMesh* mesh)
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
                const auto& tangent = mesh->mTangents[face.mIndices[j]];
                tangents.push_back({ tangent.x, tangent.y, tangent.z });

                const auto& bitangent = mesh->mBitangents[face.mIndices[j]];
                bitangents.push_back({ bitangent.x, bitangent.y, bitangent.z });
            }
        }
    }

    GLuint vao, vbo;

    std::vector<GLuint> vbos;

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
    vbos.push_back(vbo);

    // Normals
    if (hasNormals)
    {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size() * 3, normals.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(AttributeID::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AttributeID::NORMAL);
        vbos.push_back(vbo);
    }

    // TexCoords
    if (hasTexCoords)
    {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size() * 2, texCoords.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(AttributeID::TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AttributeID::TEXCOORD);
        vbos.push_back(vbo);
    }

    if (hasTangentsAndBitangents)
    {
        // Tangents
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tangents.size() * 3, tangents.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(AttributeID::TANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AttributeID::TANGENT);
        vbos.push_back(vbo);

        // Bitangents
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bitangents.size() * 3, bitangents.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(AttributeID::BITANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AttributeID::BITANGENT);
        vbos.push_back(vbo);
    }

    glBindVertexArray(0);

    glDeleteBuffers(vbos.size(), vbos.data());

    std::unique_ptr<Material> mat = nullptr;

    // Check if the mesh has materials
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        mat = processMaterial(scene, dir, material);
    }

    return std::make_unique<Mesh>(
        vao,
        GL_TRIANGLES,
        vertices.size(),
        GL_UNSIGNED_INT,
        0,
        std::move(mat)
    );
}

std::unique_ptr<Material> GameObject::processMaterial(const aiScene * scene, std::string dir, aiMaterial* material)
{
    auto mat = std::make_unique<Material>();

    // Textures
    aiString filename;

    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &filename) == AI_SUCCESS) {
        mat->SetDiffuseMap(std::make_shared<Texture>(dir + filename.C_Str()));
    }

    // if (material->GetTexture(aiTextureType_SPECULAR, 0, &filename) == AI_SUCCESS) {
    //     mat->SetSpecularMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // if (material->GetTexture(aiTextureType_AMBIENT, 0, &filename) == AI_SUCCESS) {
    //     mat->SetAmbientMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    if (material->GetTexture(aiTextureType_EMISSIVE, 0, &filename) == AI_SUCCESS) {
        mat->SetEmissiveMap(std::make_shared<Texture>(dir + filename.C_Str()));
    }

    // if (material->GetTexture(aiTextureType_HEIGHT, 0, &filename) == AI_SUCCESS) {
    //     mat->SetHeightMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    if (material->GetTexture(aiTextureType_NORMALS, 0, &filename) == AI_SUCCESS) {
        mat->SetNormalMap(std::make_shared<Texture>(dir + filename.C_Str()));
    }

    // if (material->GetTexture(aiTextureType_SHININESS, 0, &filename) == AI_SUCCESS) {
    //     mat->SetShininessMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // if (material->GetTexture(aiTextureType_OPACITY, 0, &filename) == AI_SUCCESS) {
    //     mat->SetOpacityMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // if (material->GetTexture(aiTextureType_DISPLACEMENT, 0, &filename) == AI_SUCCESS) {
    //     mat->SetDisplacementMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // if (material->GetTexture(aiTextureType_LIGHTMAP, 0, &filename) == AI_SUCCESS) {
    //     mat->SetLightMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // if (material->GetTexture(aiTextureType_REFLECTION, 0, &filename) == AI_SUCCESS) {
    //     mat->SetReflectionMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // PBR

    // if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &filename) == AI_SUCCESS) {
    //     mat->SetBaseColorMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // if (material->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &filename) == AI_SUCCESS) {
    //     mat->SetNormalCameraMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // if (material->GetTexture(aiTextureType_EMISSION_COLOR, 0, &filename) == AI_SUCCESS) {
    //     mat->SetEmissionColorMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // if (material->GetTexture(aiTextureType_METALNESS, 0, &filename) == AI_SUCCESS) {
    //     mat->SetNormalCameraMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &filename) == AI_SUCCESS) {
    //     mat->SetDiffuseRoughnessTex(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &filename) == AI_SUCCESS) {
    //     mat->SetAmbientOcclusionMap(std::make_shared<Texture>(dir + filename.C_Str()));
    // }

    // Colors
    aiColor4D aiColor;

    // Diffuse
    aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &aiColor);
    glm::vec4 diffuse((float)aiColor.r, (float)aiColor.g, (float)aiColor.b, (float)aiColor.a);
    mat->SetDiffuse(diffuse);

    // Ambient
    aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &aiColor);
    glm::vec4 ambient((float)aiColor.r, (float)aiColor.g, (float)aiColor.b, (float)aiColor.a);
    //mat->SetAmbient(ambient);

    // Specular
    aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &aiColor);
    glm::vec4 specular((float)aiColor.r, (float)aiColor.g, (float)aiColor.b, (float)aiColor.a);
    //mat->SetSpecular(specular);

    // Emissive
    aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &aiColor);
    glm::vec4 emissive((float)aiColor.r, (float)aiColor.g, (float)aiColor.b, (float)aiColor.a);
    mat->SetEmissive(emissive);

    // Transparent
    aiGetMaterialColor(material, AI_MATKEY_COLOR_TRANSPARENT, &aiColor);
    glm::vec4 transparent((float)aiColor.r, (float)aiColor.g, (float)aiColor.b, (float)aiColor.a);
    //mat->SetTransparent(transparent);

    // Reflective
    aiGetMaterialColor(material, AI_MATKEY_COLOR_REFLECTIVE, &aiColor);
    glm::vec4 reflective((float)aiColor.r, (float)aiColor.g, (float)aiColor.b, (float)aiColor.a);
    //mat->SetReflective(reflective);

    return mat;
}


