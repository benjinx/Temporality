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
    // Update all children
    for (auto& gobj : _mChildren)
    {
        gobj->Update(dt);
    }
}

void GameObject::Render()
{
    // Render all children
    for (auto& gobj : _mChildren)
    {
        gobj->Render();
    }

    if (_mShader && _mModel)
    {
        _mModel->Render(_mShader, GetWorldTransform());
    }
}

GameObject* GameObject::GetGameObject(std::string name)
{
    for (auto& gobj : _mChildren)
    {
        if (gobj->GetName() == name)
        {
            return gobj.get();
        }
    }

    for (auto& gobj : _mChildren)
    {
        auto tmp = gobj->GetGameObject(name);
        if (tmp)
        {
            return tmp;
        }
    }

    return nullptr;
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

    // Process Cameras
    std::unordered_map<std::string, std::unique_ptr<GameObject>> mapOfCameras;
    for (int i = 0; i < scene->mNumCameras; ++i)
    {
        auto cam = scene->mCameras[i];
        auto camera = std::make_unique<Camera>();

        camera->SetClip(glm::vec2(cam->mClipPlaneNear, cam->mClipPlaneFar));
        camera->SetFOVX(cam->mHorizontalFOV);
        camera->SetLookAt(glm::vec3(cam->mLookAt.x, cam->mLookAt.y, cam->mLookAt.z));
        camera->SetPosition(glm::vec3(cam->mPosition.x, cam->mPosition.y, cam->mPosition.z));
        camera->SetUp(glm::vec3(cam->mUp.x, cam->mUp.y, cam->mUp.z));

        if (camera->GetName() == "Main_Camera")
        {
            App::Inst()->SetCurrentCamera(camera.get());
        }
        else if (scene->mNumCameras == 1)
        {
            App::Inst()->SetCurrentCamera(camera.get());
        }

        mapOfCameras.emplace(cam->mName.data, std::move(camera));
    }

    // Process Lights
    std::unordered_map<std::string, std::unique_ptr<GameObject>> mapOfLights;

    for (int i = 0; i < scene->mNumLights; ++i)
    {
        auto lit = scene->mLights[i];
        
        if (lit->mType == aiLightSource_DIRECTIONAL)
        {
            auto dirLight = std::make_unique<DirectionalLight>();
            dirLight->SetPosition(glm::vec3(lit->mPosition.x, lit->mPosition.y, lit->mPosition.z));
            dirLight->SetColor(lit->mColorDiffuse.r, lit->mColorDiffuse.g, lit->mColorDiffuse.b);
            dirLight->SetDirection(glm::vec3(lit->mDirection.x, lit->mDirection.y, lit->mDirection.z));
            mapOfLights.emplace(lit->mName.data, std::move(dirLight));
        }
        else if (lit->mType == aiLightSource_POINT)
        {
            auto pointLight = std::make_unique<PointLight>();
            pointLight->SetPosition(glm::vec3(lit->mPosition.x, lit->mPosition.y, lit->mPosition.z));
            pointLight->SetColor(lit->mColorDiffuse.r, lit->mColorDiffuse.g, lit->mColorDiffuse.b);
            pointLight->SetConstant(lit->mAttenuationConstant);
            pointLight->SetLinear(lit->mAttenuationLinear);
            pointLight->SetQuadratic(lit->mAttenuationQuadratic);
            mapOfLights.emplace(lit->mName.data, std::move(pointLight));
        }
        else if (lit->mType == aiLightSource_SPOT)
        {
            auto spotLight = std::make_unique<SpotLight>();
            spotLight->SetPosition(glm::vec3(lit->mPosition.x, lit->mPosition.y, lit->mPosition.z));
            spotLight->SetColor(lit->mColorDiffuse.r, lit->mColorDiffuse.g, lit->mColorDiffuse.b);
            spotLight->SetDirection(glm::vec3(lit->mDirection.x, lit->mDirection.y, lit->mDirection.z));
            spotLight->SetCutOff(lit->mAngleInnerCone);
            spotLight->SetOuterCutOff(lit->mAngleOuterCone);
            mapOfLights.emplace(lit->mName.data, std::move(spotLight));
        }
        // No support for ambient and area lights yet.
        //else if (lit->mType == aiLightSource_AMBIENT)
        //{

        //}
        //else if (lit->mType == aiLightSource_AREA)
        //{

        //}
    }

    AddChild(processNode(scene, dir, scene->mRootNode, mapOfLights, mapOfCameras));
    
    return true;
}

std::unique_ptr<GameObject> GameObject::processNode(const aiScene * scene, std::string dir, aiNode* node, std::unordered_map<std::string, std::unique_ptr<GameObject>>& mapOfLights, std::unordered_map<std::string, std::unique_ptr<GameObject>>& mapOfCameras)
{
    std::unique_ptr<GameObject> gobj = nullptr;

    auto lightIt = mapOfLights.find(node->mName.data);
    if (lightIt != mapOfLights.end())
    {
        gobj = std::move(lightIt->second);
        mapOfLights.erase(lightIt);
    }

    auto cameraIt = mapOfCameras.find(node->mName.data);
    if (cameraIt != mapOfCameras.end())
    {
        gobj = std::move(cameraIt->second);
        mapOfCameras.erase(cameraIt);
    }

    if (!gobj)
    {
        gobj = std::make_unique<GameObject>();
    }

    gobj->SetName(node->mName.data);
    LogInfo("Name: %s\n", node->mName.data);

    aiVector3D scale, pos;
    aiQuaternion rot;
    node->mTransformation.Decompose(scale, rot, pos);

    gobj->SetScale(glm::vec3(scale.x, scale.y, scale.z));
    gobj->SetRotation(glm::quat(rot.w, rot.x, rot.y, rot.z));
    gobj->SetPosition(glm::vec3(pos.x, pos.y, pos.z));

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

        // Set default shader
        gobj->SetShader(App::Inst()->GetShader("defaultLighting"));
    }

    // Process the childrens meshes
    for (int i = 0; i < node->mNumChildren; i++)
    {
        gobj->AddChild(processNode(scene, dir, node->mChildren[i], mapOfLights, mapOfCameras));
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

    // glDeleteBuffers(vbos.size(), vbos.data());

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
        vertices.size() * 3,
        std::move(mat)
    );
}

std::unique_ptr<Material> GameObject::processMaterial(const aiScene * scene, std::string dir, aiMaterial* material)
{
    auto mat = std::make_unique<Material>();

    // Textures
    aiString filename;

    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &filename) == AI_SUCCESS) {
        
        auto tex = processTexture(scene, dir, filename);
        if (tex)
        {
            mat->SetDiffuseMap(std::move(tex));
        }
        else
        {
            LogWarn("Issue loading texture: %s\n", filename.data);
        }
    }

     //if (material->GetTexture(aiTextureType_SPECULAR, 0, &filename) == AI_SUCCESS) {
     //   auto tex = processTexture(scene, dir, filename);
     //   if (tex)
     //   {
     //       mat->SetSpecularMap(std::move(tex));
     //   }
     //   else
     //   {
     //       LogWarn("Issue loading texture: %s\n", filename.data);
     //   }
     //}

     //if (material->GetTexture(aiTextureType_AMBIENT, 0, &filename) == AI_SUCCESS) {
     //  auto tex = processTexture(scene, dir, filename);
     //  if (tex)
     //  {
     //      mat->SetAmbientMap(std::move(tex));
     //  }
     //  else
     //  {
     //      LogWarn("Issue loading texture: %s\n", filename.data);
     //  }
     //}

    if (material->GetTexture(aiTextureType_EMISSIVE, 0, &filename) == AI_SUCCESS) {
        auto tex = processTexture(scene, dir, filename);
        if (tex)
        {
            mat->SetEmissiveMap(std::move(tex));
        }
        else
        {
            LogWarn("Issue loading texture: %s\n", filename.data);
        }
    }

     //if (material->GetTexture(aiTextureType_HEIGHT, 0, &filename) == AI_SUCCESS) {
     // auto tex = processTexture(scene, dir, filename);
     // if (tex)
     // {
     //     mat->SetHeightMap(std::move(tex));
     // }
     // else
     // {
     //     LogWarn("Issue loading texture: %s\n", filename.data);
     // }
     //}

    if (material->GetTexture(aiTextureType_NORMALS, 0, &filename) == AI_SUCCESS) {

        auto tex = processTexture(scene, dir, filename);
        if (tex)
        {
            mat->SetNormalMap(std::move(tex));
        }
        else
        {
            LogWarn("Issue loading texture: %s\n", filename.data);
        }
    }

     //if (material->GetTexture(aiTextureType_SHININESS, 0, &filename) == AI_SUCCESS) {
     //    auto tex = processTexture(scene, dir, filename);
     //    if (tex)
     //    {
     //        mat->SetShininessMap(std::move(tex));
     //    }
     //    else
     //    {
     //        LogWarn("Issue loading texture: %s\n", filename.data);
     //    }
     //}

    // if (material->GetTexture(aiTextureType_OPACITY, 0, &filename) == AI_SUCCESS) {
    //    auto tex = processTexture(scene, dir, filename);
    //    if (tex)
    //    {
    //        mat->SetOpacityMap(std::move(tex));
    //    }
    //    else
    //    {
    //        LogWarn("Issue loading texture: %s\n", filename.data);
    //    }
    // }

    // if (material->GetTexture(aiTextureType_DISPLACEMENT, 0, &filename) == AI_SUCCESS) {
    //    auto tex = processTexture(scene, dir, filename);
    //    if (tex)
    //    {
    //        mat->SetDisplacementMap(std::move(tex));
    //    }
    //    else
    //    {
    //        LogWarn("Issue loading texture: %s\n", filename.data);
    //    }
    // }

    // if (material->GetTexture(aiTextureType_LIGHTMAP, 0, &filename) == AI_SUCCESS) {
    //    auto tex = processTexture(scene, dir, filename);
    //    if (tex)
    //    {
    //        mat->SetLightMap(std::move(tex));
    //    }
    //    else
    //    {
    //        LogWarn("Issue loading texture: %s\n", filename.data);
    //    }
    // }

    // if (material->GetTexture(aiTextureType_REFLECTION, 0, &filename) == AI_SUCCESS) {
    //    auto tex = processTexture(scene, dir, filename);
    //    if (tex)
    //    {
    //        mat->SetReflectionMap(std::move(tex));
    //    }
    //    else
    //    {
    //        LogWarn("Issue loading texture: %s\n", filename.data);
    //    }
    // }

    // PBR

    // if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &filename) == AI_SUCCESS) {
    //    auto tex = processTexture(scene, dir, filename);
    //    if (tex)
    //    {
    //        mat->SetBaseColorMap(std::move(tex));
    //    }
    //    else
    //    {
    //        LogWarn("Issue loading texture: %s\n", filename.data);
    //    }
    // }

    // if (material->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &filename) == AI_SUCCESS) {
    //    auto tex = processTexture(scene, dir, filename);
    //    if (tex)
    //    {
    //        mat->SetNormalCameraMap(std::move(tex));
    //    }
    //    else
    //    {
    //        LogWarn("Issue loading texture: %s\n", filename.data);
    //    }
    // }

    // if (material->GetTexture(aiTextureType_EMISSION_COLOR, 0, &filename) == AI_SUCCESS) {
    //    auto tex = processTexture(scene, dir, filename);
    //    if (tex)
    //    {
    //        mat->SetEmissionColorMap(std::move(tex));
    //    }
    //    else
    //    {
    //        LogWarn("Issue loading texture: %s\n", filename.data);
    //    }
    // }

    // if (material->GetTexture(aiTextureType_METALNESS, 0, &filename) == AI_SUCCESS) {
    //    auto tex = processTexture(scene, dir, filename);
    //    if (tex)
    //    {
    //        mat->SetNormalCameraMap(std::move(tex));
    //    }
    //    else
    //    {
    //        LogWarn("Issue loading texture: %s\n", filename.data);
    //    }
    // }

    // if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &filename) == AI_SUCCESS) {
    //    auto tex = processTexture(scene, dir, filename);
    //    if (tex)
    //    {
    //        mat->SetDiffuseRoughnessTex(std::move(tex));
    //    }
    //    else
    //    {
    //        LogWarn("Issue loading texture: %s\n", filename.data);
    //    }
    // }

    // if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &filename) == AI_SUCCESS) {
    //    auto tex = processTexture(scene, dir, filename);
    //    if (tex)
    //    {
    //        mat->SetAmbientOcclusionMap(std::move(tex));
    //    }
    //    else
    //    {
    //        LogWarn("Issue loading texture: %s\n", filename.data);
    //    }
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

std::unique_ptr<Texture> GameObject::processTexture(const aiScene * scene, std::string dir, const aiString& filename)
{
    // Binary Check
    if (filename.data[0] == '*')
    {
        int index = std::stoi(filename.data + 1);
        aiTexture* texture = scene->mTextures[index];
        unsigned char * data = (unsigned char*)texture->pcData;
        return std::make_unique<Texture>(Texture(data, glm::ivec2(texture->mWidth, texture->mHeight)));
    }
    else
    {
       return std::make_unique<Texture>(Texture(dir + filename.C_Str()));
    }
}