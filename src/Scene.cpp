#include "Scene.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <Log.hpp>
#include <Material.hpp>
#include <Model.hpp>
#include <Shader.hpp>
#include <Utils.hpp>

#include <imgui/imgui.h>

#include <nlohmann/json.hpp>

#include <stb/stb_image.h>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tinygltf/tiny_gltf.h>

bool Scene::_sShowAxis = false;

Scene::~Scene() {
    delete _mSceneAxis;
}

void Scene::Start()
{
    _mSceneAxis = new Axis();
}

void Scene::Update(float dt)
{
    // Use default shader
    App* app = App::Inst();
    Shader* defaultLighting = app->GetShader("defaultLighting");

    defaultLighting->Use();
    glm::vec3 defaultLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    defaultLighting->SetVec3("lightColor", defaultLightColor);

    glm::vec4 defaultLightPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    defaultLighting->SetVec4("lightPosition", defaultLightPosition);

    //for (auto& gobj : _mGameObjects)
    //{
    //    // Update gobjs
    //    gobj.second->Update(dt);
    //}

    for (auto& gobj : _mChildren)
    {
        // Need to also update myself?
        gobj->Update(dt);
    }
}

void Scene::Render()
{
    //for (auto& gameObject : _mGameObjects)
    //{
    //    // Render gobjs
    //    gameObject.second->Render();
    //}

    GameObject::Render();

    if (_sShowAxis)
    {
        for (auto& gameObject : _mChildren)
        {
            // Render the gobjs axis
            _mSceneAxis->Render(gameObject->GetWorldTransform());

            // Render the scenes axis
            _mSceneAxis->Render(_mSceneTransform);
        }
    }
}

bool Scene::Load(std::string filename)
{
    bool loaded = GameObject::Load(filename);

    if (loaded)
        return true;
    
    return false;
}

GameObject* Scene::AddGameObject(std::string name, std::unique_ptr<GameObject> gameObject)
{
    gameObject->SetName(name);
    _mChildren.push_back(std::move(gameObject));
    return _mChildren.back().get();
}

GameObject* Scene::AddGameObject()
{
    _mChildren.push_back(std::make_unique<GameObject>());
    return _mChildren.back().get();
}

GameObject* Scene::AddGameObject(std::string name)
{
    _mChildren.push_back(std::make_unique<GameObject>());
    _mChildren.back()->SetName(name);
    return _mChildren.back().get();
}

GameObject* Scene::AddGameObject(std::unique_ptr<GameObject> gobj)
{
    _mChildren.push_back(std::move(gobj));
    return _mChildren.back().get();
}

GameObject* Scene::GetGameObject(std::string name)
{
    for (auto& gobj : _mChildren)
    {
        if (gobj->GetName() == name)
        {
            return gobj.get();
        }
    }

    return nullptr;
}

void Scene::Options()
{
    ImGui::Checkbox("Show GameObject Axis", &_sShowAxis);
}