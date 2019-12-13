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
            _mDir = p;
        }

        if (loaded) break;
    }

    if (!loaded)
    {
        LogError("Error: Loading %s in Assimp.\n", fullPath);
    }

    _mScene = scene;

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

        if (node->mNumChildren > 0)
        {
            // Process the childrens meshes
            for (int i = 0; i < node->mNumMeshes; i++)
            {
                gobj->AddChild(processNode(node->mChildren[i]).get());
            }
        }
    }

    return std::make_shared<GameObject>(std::move(gobj));
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
                const auto& tangent = mesh->mTangents[face.mIndices[j]];
                tangents.push_back({ tangent.x, tangent.y, tangent.z });

                const auto& bitangent = mesh->mBitangents[face.mIndices[j]];
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

    Material* mat;
    // Check if the mesh has materials
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = _mScene->mMaterials[mesh->mMaterialIndex];
        mat = processMaterials(material);
    }
    auto m = std::make_shared<Material>(std::move(mat));

    // Create a new mesh
    Mesh* newMesh = new Mesh(vao,
        GL_TRIANGLES,
        vertices.size(),
        GL_UNSIGNED_INT,
        NULL,
        m);

    return newMesh;
}

Material* GameObject::processMaterials(aiMaterial* material)
{
    Material* mat = new Material();

    // Textures
    aiString path;

    // Diffuse Texture
    std::string diffuseTex = GetMaterialTextureName(material, aiTextureType_DIFFUSE, _mDir);
    auto dif = std::make_shared<Texture>(diffuseTex);
    mat->SetDiffuseMap(dif);

    // Specular Texture
    std::string specularTex = GetMaterialTextureName(material, aiTextureType_SPECULAR, _mDir);
    //auto spec = std::make_shared<Texture>(specularTex);
    //mat->SetSpecularMap(spec);

    // Ambient Texture
    std::string ambientTex = GetMaterialTextureName(material, aiTextureType_AMBIENT, _mDir);
    //auto amb = std::make_shared<Texture>(ambientTex);
    //mat->SetAmbientMap(amb);

    // Emissive Texture
    std::string emissiveTex = GetMaterialTextureName(material, aiTextureType_EMISSIVE, _mDir);
    auto emi = std::make_shared<Texture>(emissiveTex);
    mat->SetEmissiveMap(emi);

    // Height Texture
    std::string heightTex = GetMaterialTextureName(material, aiTextureType_HEIGHT, _mDir);
    //auto hei = std::make_shared<Texture>(heightTex);
    //mat->SetHeightMap(hei);

    // Normal Texture
    std::string normalTex = GetMaterialTextureName(material, aiTextureType_NORMALS, _mDir);
    auto nor = std::make_shared<Texture>(normalTex);
    mat->SetNormalMap(nor);

    // Shininess Texture
    std::string shininessTex = GetMaterialTextureName(material, aiTextureType_SHININESS, _mDir);
    //auto shi = std::make_shared<Texture>(shininessTex);
    //mat->SetShininessMap(shi);

    // Opacity Texture
    std::string opacityTex = GetMaterialTextureName(material, aiTextureType_OPACITY, _mDir);
    //auto opa = std::make_shared<Texture>(opacityTex);
    //mat->SetOpacityMap(opa);

    // Displacement Texture
    std::string displacementTex = GetMaterialTextureName(material, aiTextureType_DISPLACEMENT, _mDir);
    //auto opa = std::make_shared<Texture>(opacityTex);
    //mat->SetOpacityMap(opa);

    // LightMap Texture
    std::string lightMapTex = GetMaterialTextureName(material, aiTextureType_LIGHTMAP, _mDir);
    //auto lig = std::make_shared<Texture>(lightMapTex);
    //mat->SetLightMap(lig);

    // Reflection Texture
    std::string reflectionTex = GetMaterialTextureName(material, aiTextureType_REFLECTION, _mDir);
    //auto ref = std::make_shared<Texture>(reflectionTex);
    //mat->SetReflectionMap(ref);

    ///
    /// PBR Materials
    ///
    // Base Color Texture
    std::string baseColorTex = GetMaterialTextureName(material, aiTextureType_BASE_COLOR, _mDir);
    //auto col = std::make_shared<Texture>(baseColorTex);
    //mat->SetBaseColorMap(col);

    // Normal Camera Texture
    std::string normalCameraTex = GetMaterialTextureName(material, aiTextureType_NORMAL_CAMERA, _mDir);
    //auto ncm = std::make_shared<Texture>(normalCameraTex);
    //mat->SetNormalCameraMap(ncm);

    // Emission Color Texture
    std::string emissionColorTex = GetMaterialTextureName(material, aiTextureType_EMISSION_COLOR, _mDir);
    //auto emc = std::make_shared<Texture>(emissionColorTex);
    //mat->SetEmissionColorMap(emc);

    // Metalness Texture
    std::string metalnessTex = GetMaterialTextureName(material, aiTextureType_METALNESS, _mDir);
    //auto met = std::make_shared<Texture>(metalnessTex);
    //mat->SetNormalCameraMap(met);

    // Diffuse Roughness Texture
    std::string diffuseRoughnessTex = GetMaterialTextureName(material, aiTextureType_DIFFUSE_ROUGHNESS, _mDir);
    //auto drm = std::make_shared<Texture>(diffuseRoughnessTex);
    //mat->SetDiffuseRoughnessTex(drm);

    // Ambient Occlusion Texture
    std::string ambientOcclusionTex = GetMaterialTextureName(material, aiTextureType_AMBIENT_OCCLUSION, _mDir);
    //auto aot = std::make_shared<Texture>(ambientOcclusionTex);
    //mat->SetAmbientOcclusionMap(aot);

    ///
    ///
    ///

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

std::string GameObject::GetMaterialTextureName(aiMaterial* material, aiTextureType type, std::string dirname)
{
    std::string texName;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        if (material->GetTexture(type, i, &str) == AI_SUCCESS)
        {
            texName = dirname + str.C_Str();
        }
        else
        {
            LogWarn("Could not find texture: %s\n", str.C_Str());
            texName = "";
        }
    }
    return texName;
}


